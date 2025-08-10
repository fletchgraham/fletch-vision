#include "DepthEstimator.h"
#include <fstream>

DepthEstimator::DepthEstimator() : modelLoaded(false) {
}

DepthEstimator::~DepthEstimator() {
    // Cleanup if needed
}

bool DepthEstimator::initialize(const std::string& modelPath) {
    try {
        std::cout << "Loading MiDaS depth estimation model from: " << modelPath << std::endl;
        
        // Check if file exists
        std::ifstream file(modelPath);
        if (!file.good()) {
            std::cerr << "❌ Error: Model file does not exist: " << modelPath << std::endl;
            return false;
        }
        file.close();
        
        // Load the ONNX model (based on iwatake2222 implementation)
        dnnNet = cv::dnn::readNetFromONNX(modelPath);
        
        if (dnnNet.empty()) {
            std::cerr << "❌ Error: Could not load depth estimation model from " << modelPath << std::endl;
            return false;
        }
        
        // Set backend and target (same as working example)
        dnnNet.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
        dnnNet.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
        
        // Display model information
        for (const auto& layerName : dnnNet.getUnconnectedOutLayersNames()) {
            std::cout << "Output layer: " << layerName << std::endl;
        }
        
        modelLoaded = true;
        std::cout << "✅ MiDaS depth estimation model loaded successfully!" << std::endl;
        
        return true;
    } catch (const cv::Exception& e) {
        std::cerr << "❌ Error loading depth estimation model: " << e.what() << std::endl;
        modelLoaded = false;
        return false;
    }
}

cv::Mat DepthEstimator::estimateDepth(const cv::Mat& inputImage) {
    if (!modelLoaded || inputImage.empty()) {
        return cv::Mat();
    }
    
    try {
        // Preprocess input image (based on iwatake2222 implementation)
        cv::Mat blobInput;
        preProcess(inputImage, blobInput);
        
        // Run inference
        std::vector<cv::String> outputNames = dnnNet.getUnconnectedOutLayersNames();
        std::vector<cv::Mat> outputMatList;
        inference(blobInput, outputNames, outputMatList);
        
        if (outputMatList.empty()) {
            std::cerr << "❌ Error: No output from depth estimation model" << std::endl;
            return cv::Mat();
        }
        
        // Get depth map from network output
        cv::Mat matDepth = outputMatList[0];
        
        // If output is 3D (1, H, W), reshape to 2D (H, W)
        if (matDepth.dims == 3) {
            std::vector<int> sizes = {matDepth.size[1], matDepth.size[2]};
            matDepth = matDepth.reshape(0, sizes);
        }
        
        return matDepth.clone();
    } catch (const cv::Exception& e) {
        std::cerr << "❌ Error during depth estimation: " << e.what() << std::endl;
        return cv::Mat();
    }
}

bool DepthEstimator::normalizeMinMax(const cv::Mat& matDepth, cv::Mat& matDepthNormalized) {
    // Normalize to uint8_t(0-255) (Far = 255, Near = 0)
    // Based on iwatake2222 implementation
    matDepthNormalized = cv::Mat(kModelInputHeight, kModelInputWidth, CV_8UC1);
    double depthMin, depthMax;
    cv::minMaxLoc(matDepth, &depthMin, &depthMax);
    double range = depthMax - depthMin;
    if (range > 0) {
        matDepth.convertTo(matDepthNormalized, CV_8UC1, 255.0 / range, (-255.0 * depthMin) / range);
        matDepthNormalized = 255 - matDepthNormalized;
        return true;
    } else {
        return false;
    }
}

cv::Mat DepthEstimator::createDepthHeatMap(const cv::Mat& depthMap) {
    if (depthMap.empty()) {
        return cv::Mat();
    }
    
    cv::Mat normalizedDepth;
    if (!normalizeMinMax(depthMap, normalizedDepth)) {
        return cv::Mat();
    }
    
    cv::Mat heatMap;
    cv::applyColorMap(normalizedDepth, heatMap, cv::COLORMAP_JET);
    
    return heatMap;
}

cv::Mat DepthEstimator::overlayDepthHeatMap(const cv::Mat& originalImage, const cv::Mat& depthMap, float alpha) {
    if (originalImage.empty() || depthMap.empty()) {
        return originalImage;
    }
    
    // Create heat map
    cv::Mat heatMap = createDepthHeatMap(depthMap);
    if (heatMap.empty()) {
        return originalImage;
    }
    
    // Resize heat map to match original image size
    cv::Mat resizedHeatMap;
    cv::resize(heatMap, resizedHeatMap, originalImage.size());
    
    // Blend original image with heat map
    cv::Mat result;
    cv::addWeighted(originalImage, 1.0f - alpha, resizedHeatMap, alpha, 0, result);
    
    return result;
}

void DepthEstimator::preProcess(const cv::Mat& imageInput, cv::Mat& blobInput) {
    // Based on iwatake2222 implementation
    cv::Mat imageNormalize;
    cv::resize(imageInput, imageNormalize, cv::Size(kModelInputWidth, kModelInputHeight));
    cv::cvtColor(imageNormalize, imageNormalize, cv::COLOR_BGR2RGB);
    imageNormalize.convertTo(imageNormalize, CV_32FC3, 1.0 / 255.0);
    
    // Apply ImageNet normalization
    cv::subtract(imageNormalize, cv::Scalar(cv::Vec<float, 3>(kMeanList[0], kMeanList[1], kMeanList[2])), imageNormalize);
    cv::divide(imageNormalize, cv::Scalar(cv::Vec<float, 3>(kNormList[0], kNormList[1], kNormList[2])), imageNormalize);
    
    // Convert NHWC(image) -> NCHW (blob)
    blobInput = cv::dnn::blobFromImage(imageNormalize);
}

void DepthEstimator::inference(const cv::Mat& blobInput, const std::vector<cv::String>& outputNameList, std::vector<cv::Mat>& outputMatList) {
    dnnNet.setInput(blobInput);
    dnnNet.forward(outputMatList, outputNameList);
}
