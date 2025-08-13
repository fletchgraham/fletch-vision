#include "WebcamCapture.h"

WebcamCapture::WebcamCapture() : active(false), preferredWidth(640), preferredHeight(480) {
}

WebcamCapture::~WebcamCapture() {
    release();
}

bool WebcamCapture::initialize() {
    std::cout << "Attempting to open camera..." << std::endl;
    return tryInitializeCamera();
}

bool WebcamCapture::tryInitializeCamera() {
    // Try different camera backends and indices
    std::vector<int> backends = {cv::CAP_AVFOUNDATION, cv::CAP_ANY};
    std::vector<int> indices = {0, 1, 2};
    
    for (int backend : backends) {
        for (int index : indices) {
            std::cout << "Trying camera index " << index << " with backend " << backend << std::endl;
            cap.open(index, backend);
            
            if (cap.isOpened()) {
                std::cout << "Camera opened successfully with index " << index << " and backend " << backend << std::endl;
                
                // Set camera properties
                cap.set(cv::CAP_PROP_FRAME_WIDTH, preferredWidth);
                cap.set(cv::CAP_PROP_FRAME_HEIGHT, preferredHeight);
                
                // Test if we can actually read a frame
                cv::Mat testFrame;
                if (cap.read(testFrame) && !testFrame.empty()) {
                    std::cout << "✅ Webcam initialized successfully!" << std::endl;
                    std::cout << "Frame size: " << testFrame.cols << "x" << testFrame.rows << std::endl;
                    active = true;
                    return true;
                } else {
                    std::cout << "Camera opened but couldn't read frame" << std::endl;
                    cap.release();
                }
            }
        }
    }
    
    std::cerr << "❌ Error: Could not open any webcam" << std::endl;
    std::cerr << "Make sure:" << std::endl;
    std::cerr << "1. Camera permissions are granted to Terminal" << std::endl;
    std::cerr << "2. No other app is using the camera" << std::endl;
    std::cerr << "3. Camera is properly connected" << std::endl;
    active = false;
    return false;
}

bool WebcamCapture::captureFrame(cv::Mat& frame) {
    if (!active || !cap.isOpened()) {
        return false;
    }
    
    return cap.read(frame) && !frame.empty();
}

void WebcamCapture::release() {
    if (cap.isOpened()) {
        cap.release();
    }
    active = false;
}

cv::Size WebcamCapture::getFrameSize() const {
    if (!active || !cap.isOpened()) {
        return cv::Size(0, 0);
    }
    
    int width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    int height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    return cv::Size(width, height);
}

void WebcamCapture::setFrameSize(int width, int height) {
    preferredWidth = width;
    preferredHeight = height;
    
    // If already initialized, update the settings
    if (active && cap.isOpened()) {
        cap.set(cv::CAP_PROP_FRAME_WIDTH, width);
        cap.set(cv::CAP_PROP_FRAME_HEIGHT, height);
    }
}
