#include <opencv2/opencv.hpp>

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
    double width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    double height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

    // HSV color values for tracking (adjust these based on your object's color)
    int hMin = 0, sMin = 100, vMin = 100;
    int hMax = 179, sMax = 255, vMax = 255;

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

        // Draw contours on the original image
        cv::Mat image_copy = image.clone();
        cv::drawContours(image_copy, contours, -1, cv::Scalar(0, 0, 255), 2);

        // Display the actual frame (image)
        cv::imshow("Webcam", image_copy);

        // Display the x and y positions of the contours relative to the frame size
        for (const auto& contour : contours) {
            for (const auto& point : contour) {
                double relativeX = static_cast<double>(point.x) / width;
                double relativeY = static_cast<double>(point.y) / height;
                std::cout << "Relative X: " << relativeX << ", Relative Y: " << relativeY << std::endl;
            }
        }

        // Break the loop if the 'ESC' key is pressed
        if (cv::waitKey(1) == 27) {
            break;
        }
    }

    return 0;
}
