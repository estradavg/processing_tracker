#include <opencv2/opencv.hpp>

int main() {
    cv::VideoCapture cap(0, cv::CAP_V4L);  // Assuming you want to capture from the default camera
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

    // std::cout << "Width: " << width << ", Height: " << height << std::endl;

    while (true) {
        cv::Mat image;
        if (!cap.read(image)) {
            std::cerr << "Error: Could not read frame from the camera." << std::endl;
            break;
        }

        // convert image to grayscale format
        cv::Mat img_gray;
        cv::cvtColor(image, img_gray, cv::COLOR_BGR2GRAY);
        
        // apply binary treshholding
        cv::Mat thresh;
        cv::threshold(img_gray, thresh, 150, 255, cv::THRESH_BINARY);
        
        // detect the contours on the binary image using cv.CHAIN_APPROX_NONE
        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;
        cv::findContours(thresh.clone(), contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_NONE);
        

        // draw contours on the original image
        cv::Mat image_copy = image.clone();
        cv::drawContours(image_copy, contours, -1, cv::Scalar(0, 0, 255), 2);

	    // Display the actual frame (image)
        cv::imshow("Webcam", image_copy);

        // Break the loop if the 'ESC' key is pressed
        if (cv::waitKey(1) == 27) {
            break;
        }
    }

    return 0;
}
