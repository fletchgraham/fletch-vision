#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    std::cout << "=== OpenCV Camera Test ===" << std::endl;
    std::cout << "OpenCV version: " << CV_VERSION << std::endl;
    
    cv::VideoCapture cap;
    
    // Try AVFoundation backend specifically (macOS native)
    std::cout << "Trying AVFoundation backend..." << std::endl;
    cap.open(0, cv::CAP_AVFOUNDATION);
    
    if (!cap.isOpened()) {
        std::cout << "AVFoundation failed, trying default backend..." << std::endl;
        cap.open(0);
    }
    
    if (cap.isOpened()) {
        std::cout << "✅ Camera opened successfully!" << std::endl;
        
        cv::Mat frame;
        cap >> frame;
        
        if (!frame.empty()) {
            std::cout << "✅ Successfully captured frame: " << frame.cols << "x" << frame.rows << std::endl;
            std::cout << "Camera test successful! Press any key to continue..." << std::endl;
            
            // Save a test image
            cv::imwrite("test_frame.jpg", frame);
            std::cout << "Saved test frame as test_frame.jpg" << std::endl;
            
            std::cin.get();
        } else {
            std::cout << "❌ Camera opened but frame is empty" << std::endl;
        }
        
        cap.release();
    } else {
        std::cout << "❌ Failed to open camera" << std::endl;
        std::cout << "Please check:" << std::endl;
        std::cout << "1. Camera permissions for Terminal" << std::endl;
        std::cout << "2. No other apps using camera" << std::endl;
        return -1;
    }
    
    return 0;
}
