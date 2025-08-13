#pragma once

#include "IDepthEstimator.h"
#include <memory>
#include <string>

/**
 * Factory for creating depth estimators.
 * This makes it easy for any demo to create and initialize a depth estimator.
 */
class DepthEstimatorFactory {
public:
    /**
     * Create a depth estimator with the specified model path.
     * @param modelPath Path to the MiDaS ONNX model file
     * @return Unique pointer to the created depth estimator, or nullptr if creation failed
     */
    static std::unique_ptr<IDepthEstimator> create(const std::string& modelPath);
    
    /**
     * Create a depth estimator, trying multiple common model paths.
     * @return Unique pointer to the created depth estimator, or nullptr if creation failed
     */
    static std::unique_ptr<IDepthEstimator> createWithDefaultPaths();
};
