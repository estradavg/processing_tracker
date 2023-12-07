#include <iostream>
#include <opencv2/opencv.hpp>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sstream>
#include <iomanip>

int main() {
    cv::VideoCapture cap(0, cv::CAP_V4L);
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open camera." << std::endl;
        return -1;
    }

    // Set the width and height
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 720);

    // Check if the settings were applied successfully
    int width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

    // HSV color values for tracking (adjust these based on your object's color)
    int hMin = 0, sMin = 100, vMin = 100;
    int hMax = 179, sMax = 255, vMax = 255;

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

    while (true) {
        cv::Mat image;
        if (!cap.read(image)) {
            std::cerr << "Error: Could not read frame from the camera." << std::endl;
            break;
        }

        // Convert image to HSV color space
        cv::Mat hsvImage;
        cv::cvtColor(image, hsvImage, cv::COLOR_BGR2HSV);

        // Threshold based on HSV values
        cv::Mat mask;
        cv::inRange(hsvImage, cv::Scalar(hMin, sMin, vMin), cv::Scalar(hMax, sMax, vMax), mask);

        // Find contours on the binary image
        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;
        cv::findContours(mask.clone(), contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

        // Define minimum and maximum area
        double minArea = 1000.0;
        double maxArea = std::numeric_limits<double>::infinity();

        cv::Mat image_contours = image.clone();

        for (const auto& contour : contours) {
            double area = cv::contourArea(contour);

            if (minArea < area && area < maxArea) {
                double peri = cv::arcLength(contour, 1);
                std::vector<cv::Point> approx;
                cv::approxPolyDP(contour, approx, 0.02 * peri, 1);
                
                // Draw contours on the original image
                cv::drawContours(image_contours,contours, -1, cv::Scalar(255, 0, 0), 3);
                cv::Rect boundingRect = cv::boundingRect(approx);

                int x, y, w, h;
                x = boundingRect.x;
                y = boundingRect.y;
                w = boundingRect.width;
                h = boundingRect.height;

                int cx = x + (w / 2);
                int cy = y + (h / 2);

                cv::rectangle(image_contours, cv::Point(x, y), cv::Point(x + w, y + h), cv::Scalar(255, 0, 0), 2);
                cv::circle(image_contours, cv::Point(cx, cy), 5, cv::Scalar(255, 0, 0), cv::FILLED);

                std::cout << "Center X: " << cx << ", Center Y: " << cy << std::endl;
                
                if (cx != 0 && cy != 0) {
                    std::ostringstream dataStream;
                    dataStream << std::fixed << std::setprecision(2) << cx << "\n" << height - cy << "\n";
                    std::string data = dataStream.str();

                    ssize_t sentBytes = send(clientSocket, data.c_str(), data.size(), 0);

                    if (sentBytes == -1) {
                        std::cerr << "Error: Could not send data." << std::endl;
                        close(clientSocket);
                        return -1;
                    }
                }
            }
            
        }

        // Display the actual frame (image)
        cv::imshow("Image", image_contours);

        // Break the loop if the 'ESC' key is pressed
        if (cv::waitKey(1) == 27) {
            break;
        }
    }

    return 0;
}
