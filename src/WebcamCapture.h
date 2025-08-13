#pragma once

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include "IWebcamCapture.h"

/**
 * OpenCV-based webcam capture implementation.
 * Handles the complexity of initializing webcam across different backends.
 */
class WebcamCapture : public IWebcamCapture {
public:
    WebcamCapture();
    ~WebcamCapture() override;
    
    // Initialize the webcam with smart backend detection
    bool initialize() override;
    
    // Capture a frame from the webcam
    bool captureFrame(cv::Mat& frame) override;
    
    // Check if webcam is active/initialized
    bool isActive() const override { return active; }
    
    // Release the webcam
    void release() override;
    
    // Get frame dimensions
    cv::Size getFrameSize() const override;
    
    // Optional: Set frame size (call before initialize())
    void setFrameSize(int width, int height);
    
private:
    cv::VideoCapture cap;
    bool active;
    int preferredWidth;
    int preferredHeight;
    
    // Helper method to try different camera configurations
    bool tryInitializeCamera();
};
