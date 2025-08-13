#include "DepthEstimatorFactory.h"
#include "DepthEstimator.h"
#include <iostream>
#include <vector>

std::unique_ptr<IDepthEstimator> DepthEstimatorFactory::create(const std::string& modelPath) {
    std::unique_ptr<IDepthEstimator> estimator(new DepthEstimator());
    
    if (estimator->initialize(modelPath)) {
        std::cout << "✅ Created depth estimator: " << estimator->getDescription() << std::endl;
        return estimator;
    } else {
        std::cerr << "❌ Failed to initialize depth estimator with model: " << modelPath << std::endl;
        return std::unique_ptr<IDepthEstimator>();
    }
}

std::unique_ptr<IDepthEstimator> DepthEstimatorFactory::createWithDefaultPaths() {
    // Try common model paths
    std::vector<std::string> modelPaths = {
        "models/midasv2_small_256x256.onnx",
        "midasv2_small_256x256.onnx"
    };
    
    for (const std::string& path : modelPaths) {
        auto estimator = create(path);
        if (estimator) {
            return estimator;
        }
    }
    
    std::cerr << "❌ Could not initialize depth estimator with any default model path" << std::endl;
    std::cerr << "Make sure midasv2_small_256x256.onnx is available in one of these locations:" << std::endl;
    for (const std::string& path : modelPaths) {
        std::cerr << "  - " << path << std::endl;
    }
    
    return std::unique_ptr<IDepthEstimator>();
}
