#pragma once

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

class SimpleDepthEstimator {
public:
    SimpleDepthEstimator();
    ~SimpleDepthEstimator();
    
    // Initialize the depth estimation (no model required)
    bool initialize();
    
    // Estimate depth using simple computer vision techniques
    cv::Mat estimateDepth(const cv::Mat& inputImage);
    
    // Create a colorized heat map from depth data
    cv::Mat createDepthHeatMap(const cv::Mat& depthMap);
    
    // Overlay depth heat map on original image
    cv::Mat overlayDepthHeatMap(const cv::Mat& originalImage, const cv::Mat& depthMap, float alpha = 0.6f);
    
    // Check if estimator is ready
    bool isInitialized() const { return initialized; }
    
private:
    bool initialized;
    
    // Simple depth estimation using edge density and blur
    cv::Mat computeSimpleDepth(const cv::Mat& image);
};
