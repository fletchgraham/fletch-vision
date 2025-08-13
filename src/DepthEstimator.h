#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <iostream>
#include <string>
#include <array>
#include "IDepthEstimator.h"

class DepthEstimator : public IDepthEstimator {
public:
    DepthEstimator();
    ~DepthEstimator() override;
    
    // Initialize the depth estimation model
    bool initialize(const std::string& modelPath = "") override;
    
    // Estimate depth from input image and return depth map
    cv::Mat estimateDepth(const cv::Mat& inputImage) override;
    
    // Create a colorized heat map from depth data
    cv::Mat createDepthHeatMap(const cv::Mat& depthMap) override;
    
    // Overlay depth heat map on original image
    cv::Mat overlayDepthHeatMap(const cv::Mat& originalImage, const cv::Mat& depthMap, float alpha = 0.6f) override;
    
    // Check if model is loaded
    bool isInitialized() const override { return modelLoaded; }
    
    // Get description of this depth estimation method
    std::string getDescription() const override { return "MiDaS v2.1 Neural Network"; }
    
    // Normalize depth map to 0-255 range (based on iwatake2222 implementation)
    bool normalizeMinMax(const cv::Mat& matDepth, cv::Mat& matDepthNormalized);
    
private:
    cv::dnn::Net dnnNet;
    bool modelLoaded;
    
    // Model input parameters (MiDaS v2.1 small)
    static constexpr int32_t kModelInputWidth = 256;
    static constexpr int32_t kModelInputHeight = 256;
    const std::array<float, 3> kMeanList = { 0.485f, 0.456f, 0.406f };
    const std::array<float, 3> kNormList = { 0.229f, 0.224f, 0.225f };
    
    // Helper functions
    void preProcess(const cv::Mat& imageInput, cv::Mat& blobInput);
    void inference(const cv::Mat& blobInput, const std::vector<cv::String>& outputNameList, std::vector<cv::Mat>& outputMatList);
};
