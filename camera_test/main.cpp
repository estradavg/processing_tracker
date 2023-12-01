#include <opencv2/opencv.hpp>

int main() {
    cv::VideoCapture cap(0, cv::CAP_V4L);  // Assuming you want to capture from the default camera
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open camera." << std::endl;
        return -1;
    }

    // Set the width and height
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 1080);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 720);

    // Check if the settings were applied successfully
    double width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    double height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

    // std::cout << "Width: " << width << ", Height: " << height << std::endl;

    while (true) {
        cv::Mat frame;
        bool success = cap.read(frame);

        if (!success) {
            std::cerr << "Error: Could not read frame from the camera." << std::endl;
            break;
        }

        // Display the frame
        cv::imshow("Webcam", frame);

        // Break the loop if the 'ESC' key is pressed
        if (cv::waitKey(1) == 27) {
            break;
        }
    }

    return 0;
}
