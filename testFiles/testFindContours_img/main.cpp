#include <opencv2/opencv.hpp>

int main() {
    cv::Mat image = cv::imread("../images/dog_bike_car.jpg");
    // cv::Mat test = cv::Mat::zeros(cv::Size(1, 49), CV_64FC1);

    // error handling 
    if (image.empty()) { 
        std::cerr << "Error: Image Not Found !" << std::endl; 
        return -1; 
    } 
    
    // convert image to grayscale format
    cv::Mat img_gray;
    cv::cvtColor(image, img_gray, cv::COLOR_BGR2GRAY);
    
    // apply binary treshholding
    cv::Mat thresh;
    cv::threshold(img_gray, thresh, 150, 255, cv::THRESH_BINARY);
    
    // CODE WORKS FINE UNTIL HERE
    // cv::imshow("Threshold", thresh);
    // cv::waitKey(0);

    // ERROR SEGMENTATION FAULT
    //detect the contours on the binary image using cv.CHAIN_APPROX_NONE
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(thresh.clone(), contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
    

    // draw contours on the original image
    cv::Mat image_copy = image.clone();
    cv::drawContours(image_copy, contours, -1, cv::Scalar(0, 0, 255), 2);
    

    // show image
    cv::imshow("Simple approximation", image_copy);
    cv::waitKey(0);
    
    cv::imwrite("contours_simple_image.jpg", image_copy);
    cv::destroyAllWindows();

    return 0;
}
