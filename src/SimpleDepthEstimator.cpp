#include "SimpleDepthEstimator.h"

SimpleDepthEstimator::SimpleDepthEstimator() : initialized(false) {
}

SimpleDepthEstimator::~SimpleDepthEstimator() {
    // Cleanup if needed
}

bool SimpleDepthEstimator::initialize() {
    std::cout << "Initializing simple depth estimation (computer vision based)..." << std::endl;
    initialized = true;
    std::cout << "✅ Simple depth estimation initialized successfully!" << std::endl;
    return true;
}

cv::Mat SimpleDepthEstimator::computeSimpleDepth(const cv::Mat& image) {
    cv::Mat depth;
    cv::Mat gray;
    
    // Convert to grayscale
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    
    // Method 1: Use Laplacian to detect edges (closer objects often have sharper edges)
    cv::Mat laplacian;
    cv::Laplacian(gray, laplacian, CV_64F);
    cv::convertScaleAbs(laplacian, laplacian);
    
    // Method 2: Use gradient magnitude
    cv::Mat grad_x, grad_y, grad;
    cv::Sobel(gray, grad_x, CV_64F, 1, 0, 3);
    cv::Sobel(gray, grad_y, CV_64F, 0, 1, 3);
    cv::magnitude(grad_x, grad_y, grad);
    cv::convertScaleAbs(grad, grad);
    
    // Combine Laplacian and gradient information
    cv::Mat combined;
    cv::addWeighted(laplacian, 0.6, grad, 0.4, 0, combined);
    
    // Apply Gaussian blur to smooth the depth map
    cv::GaussianBlur(combined, depth, cv::Size(5, 5), 2.0);
    
    // Invert so that higher values represent closer objects
    cv::bitwise_not(depth, depth);
    
    // Normalize to 0-1 range
    cv::normalize(depth, depth, 0, 1, cv::NORM_MINMAX, CV_32F);
    
    return depth;
}

cv::Mat SimpleDepthEstimator::estimateDepth(const cv::Mat& inputImage) {
    if (!initialized || inputImage.empty()) {
        std::cerr << "❌ Error: Estimator not initialized or empty input image" << std::endl;
        return cv::Mat();
    }
    
    try {
        // Compute simple depth using computer vision techniques
        cv::Mat depthMap = computeSimpleDepth(inputImage);
        return depthMap;
    } catch (const cv::Exception& e) {
        std::cerr << "❌ Error during depth estimation: " << e.what() << std::endl;
        return cv::Mat();
    }
}

cv::Mat SimpleDepthEstimator::createDepthHeatMap(const cv::Mat& depthMap) {
    if (depthMap.empty()) {
        return cv::Mat();
    }
    
    cv::Mat heatMap;
    cv::Mat depthNormalized;
    
    // Ensure depth map is in the right format
    if (depthMap.type() != CV_8UC1) {
        depthMap.convertTo(depthNormalized, CV_8UC1, 255.0);
    } else {
        depthNormalized = depthMap.clone();
    }
    
    // Apply color map for heat map visualization
    // COLORMAP_VIRIDIS provides a nice blue->green->yellow progression
    // Other options: COLORMAP_HOT, COLORMAP_JET, COLORMAP_PLASMA, COLORMAP_INFERNO
    cv::applyColorMap(depthNormalized, heatMap, cv::COLORMAP_VIRIDIS);
    
    return heatMap;
}

cv::Mat SimpleDepthEstimator::overlayDepthHeatMap(const cv::Mat& originalImage, const cv::Mat& depthMap, float alpha) {
    if (originalImage.empty() || depthMap.empty()) {
        return originalImage;
    }
    
    // Create heat map from depth
    cv::Mat heatMap = createDepthHeatMap(depthMap);
    
    if (heatMap.empty()) {
        return originalImage;
    }
    
    // Ensure both images have the same size
    cv::Mat resizedHeatMap;
    if (heatMap.size() != originalImage.size()) {
        cv::resize(heatMap, resizedHeatMap, originalImage.size());
    } else {
        resizedHeatMap = heatMap;
    }
    
    // Blend the original image with the heat map
    cv::Mat result;
    cv::addWeighted(originalImage, 1.0 - alpha, resizedHeatMap, alpha, 0, result);
    
    return result;
}
