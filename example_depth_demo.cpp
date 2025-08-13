// Example demonstrating how to use depth estimation in any demo
// This shows how the refactored interface makes depth estimation reusable

#include <iostream>
#include <opencv2/opencv.hpp>
#include "DepthEstimatorFactory.h"

int main() {
    std::cout << "=== Depth Estimation Demo ===" << std::endl;
    
    // Easy to create a depth estimator - any demo can do this
    auto depthEstimator = DepthEstimatorFactory::createWithDefaultPaths();
    
    if (!depthEstimator) {
        std::cerr << "Could not create depth estimator. Make sure MiDaS model is available." << std::endl;
        return -1;
    }
    
    std::cout << "Using: " << depthEstimator->getDescription() << std::endl;
    
    // Open webcam
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open camera" << std::endl;
        return -1;
    }
    
    cv::Mat frame;
    std::cout << "Press 'q' to quit, 'SPACE' to process frame" << std::endl;
    
    while (true) {
        cap >> frame;
        if (frame.empty()) break;
        
        cv::imshow("Original", frame);
        
        int key = cv::waitKey(1) & 0xFF;
        if (key == 'q') break;
        
        if (key == ' ') {  // Spacebar
            std::cout << "Processing depth estimation..." << std::endl;
            
            // Easy depth estimation - just call the interface
            cv::Mat depthMap = depthEstimator->estimateDepth(frame);
            if (!depthMap.empty()) {
                cv::Mat heatMap = depthEstimator->createDepthHeatMap(depthMap);
                cv::Mat overlay = depthEstimator->overlayDepthHeatMap(frame, depthMap);
                
                cv::imshow("Depth Heat Map", heatMap);
                cv::imshow("Depth Overlay", overlay);
            }
        }
    }
    
    cv::destroyAllWindows();
    return 0;
}
