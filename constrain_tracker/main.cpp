#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <functional>
#include <iostream>
#include <stdio.h>
#include <string>
#include <tuple>
#include <map>

#include <sstream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

std::tuple<cv::Mat, cv::Mat> update(cv::Mat img, std::map<std::string, int> myColor = {}) {
    cv::Mat imgColor;
    cv::Mat mask;

    if (myColor.empty()) {
        // Handle the case when myColor is not provided
        return std::make_tuple(imgColor, mask);
    }

    cv::Mat imgHSV;
    cv::cvtColor(img, imgHSV, cv::COLOR_BGR2HSV);

    cv::Scalar lower(myColor["hmin"], myColor["smin"], myColor["vmin"]);
    cv::Scalar upper(myColor["hmax"], myColor["smax"], myColor["vmax"]);

    cv::inRange(imgHSV, lower, upper, mask);
    cv::bitwise_and(img, img, imgColor, mask);
    
    std::cout << "Succes 1" << std::endl;

    return std::make_tuple(imgColor, mask);
}


std::tuple<cv::Mat, std::vector<std::map<std::string, cv::Mat>>> findContours(cv::Mat img, cv::Mat imgPre,
    double minArea = 1000, double maxArea = std::numeric_limits<double>::infinity(),
    bool sort = true, std::vector<int> filter = {}, bool drawCon = true,
    cv::Scalar c = cv::Scalar(255, 0, 0), cv::Scalar ct = cv::Scalar(255, 0, 255),
    int retrType = cv::RETR_EXTERNAL, int approxType = cv::CHAIN_APPROX_NONE) {

    std::vector<std::map<std::string, cv::Mat>> conFound;
    cv::Mat imgContours = img.clone();
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;

    cv::findContours(imgPre, contours, hierarchy, retrType, approxType);

    for (const auto& cnt : contours) {
        double area = cv::contourArea(cnt);
        if (minArea < area && area < maxArea) {
            double peri = cv::arcLength(cnt, true);
            std::vector<cv::Point> approx;
            cv::approxPolyDP(cnt, approx, 0.02 * peri, true);

            if (filter.empty() || approx.size() == filter[0]) {
                if (drawCon) {
                    cv::drawContours(imgContours, std::vector<std::vector<cv::Point>>{cnt}, -1, c, 3);
                    cv::Rect boundingRect = cv::boundingRect(approx);
                    cv::putText(imgContours, std::to_string(approx.size()), cv::Point(boundingRect.x, boundingRect.y - 10),
                        cv::FONT_HERSHEY_SIMPLEX, 0.5, ct, 2);
                }

                int x, y, w, h;
                cv::Rect boundingRect = cv::boundingRect(approx);
                x = boundingRect.x;
                y = boundingRect.y;
                w = boundingRect.width;
                h = boundingRect.height;

                int cx = x + (w / 2);
                int cy = y + (h / 2);

                cv::rectangle(imgContours, cv::Point(x, y), cv::Point(x + w, y + h), c, 2);
                cv::circle(imgContours, cv::Point(cx, cy), 5, c, cv::FILLED);

                conFound.push_back({{"cnt", cv::Mat(cnt)}, {"area", cv::Mat(std::vector<double>{area})},
                                    {"bbox", cv::Mat(std::vector<int>{x, y, w, h})},
                                    {"center", cv::Mat(std::vector<int>{cx, cy})}});
            }
        }
    }

    if (sort) {
        std::sort(conFound.begin(), conFound.end(), [](const auto& a, const auto& b) -> bool {
            return a.at("area").template at<double>(0, 0) > b.at("area").template at<double>(0, 0);
        });
    }

    return std::make_tuple(imgContours, conFound);
}

int main() 
{
    cv::VideoCapture cap(0, cv::CAP_V4L); 
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open camera." << std::endl;
        return -1;
    }

    cap.set(cv::CAP_PROP_FRAME_WIDTH, 1080);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 720);

    // Capture the first frame
    cv::Mat img;
    bool success = cap.read(img);
    if (!success) {
        std::cerr << "Error: Could not read frame from the camera." << std::endl;
        return -1;
    }

    double h = 1280;
    double w = 720;

    std::map<std::string, int> hsvVals = {{"hmin", 37}, {"smin", 34}, {"vmin", 0},
                                          {"hmax", 105}, {"smax", 255}, {"vmax", 255}};

    // Socket variables
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Error: Could not create socket." << std::endl;
        return -1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12001);
    inet_pton(AF_INET, "127.0.0.1", &(serverAddr.sin_addr));

    if (connect(clientSocket, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr)) == -1) {
        std::cerr << "Error: Could not connect to the server." << std::endl;
        close(clientSocket);
        return -1;
    }

    // Main loop
    while (true) {
        if (!cap.read(img)) {
            std::cerr << "Error: Could not read frame from the camera." << std::endl;
            break;
        }
        cv::Mat imgColor, mask;
        // std::tie(imgColor, mask) = update(img, hsvVals);
        
        auto [imgContour, contour] = findContours(img, mask);

        if (!contour.empty()) {
            double centerX = contour[0]["center"].at<int>(0);
            double centerY = h - contour[0]["center"].at<int>(1);
            int area = contour[0]["area"].at<int>(0);

            std::cout << "Data: " << centerX << ", " << centerY << ", " << area << std::endl;

            if (centerX != 0 && centerY != 0) {
                std::ostringstream dataStream;
                dataStream << std::fixed << std::setprecision(2) << centerX << "\n" << centerY << "\n";
                std::string data = dataStream.str();

                send(clientSocket, data.c_str(), data.size(), 0);
            }
        }

        cv::Mat imgStack;  // Implement the stackImages logic
        cv::imshow("Image", imgStack);
        
        // Break the loop if the 'ESC' key is pressed
        if (cv::waitKey(1) == 27) {
            break;
        }
        
    }

    // Close the socket
    close(clientSocket);

    return 0;
}
