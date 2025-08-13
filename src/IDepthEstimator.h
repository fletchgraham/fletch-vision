#pragma once

#include <opencv2/opencv.hpp>
#include <string>

/**
 * Abstract interface for depth estimation implementations.
 * This allows different demos to use any depth estimation approach interchangeably.
 */
class IDepthEstimator {
public:
    virtual ~IDepthEstimator() = default;
    
    // Initialize the depth estimator (may require model path for ML-based estimators)
    virtual bool initialize(const std::string& modelPath = "") = 0;
    
    // Estimate depth from input image and return depth map
    virtual cv::Mat estimateDepth(const cv::Mat& inputImage) = 0;
    
    // Create a colorized heat map from depth data
    virtual cv::Mat createDepthHeatMap(const cv::Mat& depthMap) = 0;
    
    // Overlay depth heat map on original image
    virtual cv::Mat overlayDepthHeatMap(const cv::Mat& originalImage, const cv::Mat& depthMap, float alpha = 0.6f) = 0;
    
    // Check if estimator is ready
    virtual bool isInitialized() const = 0;
    
    // Get a description of the depth estimation method
    virtual std::string getDescription() const = 0;
};
