#pragma once

#include <opencv2/opencv.hpp>
#include <string>

/**
 * Simple interface for webcam capture.
 * This allows different demos to easily access webcam functionality.
 */
class IWebcamCapture {
public:
    virtual ~IWebcamCapture() = default;
    
    // Initialize the webcam
    virtual bool initialize() = 0;
    
    // Capture a frame from the webcam
    virtual bool captureFrame(cv::Mat& frame) = 0;
    
    // Check if webcam is active/initialized
    virtual bool isActive() const = 0;
    
    // Release the webcam
    virtual void release() = 0;
    
    // Get frame dimensions
    virtual cv::Size getFrameSize() const = 0;
};
