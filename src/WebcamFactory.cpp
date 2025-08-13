#include "WebcamFactory.h"
#include "WebcamCapture.h"
#include <iostream>

std::unique_ptr<IWebcamCapture> WebcamFactory::create() {
    return create(640, 480); // Default resolution
}

std::unique_ptr<IWebcamCapture> WebcamFactory::create(int width, int height) {
    std::unique_ptr<WebcamCapture> webcam(new WebcamCapture());
    
    // Set preferred frame size before initialization
    webcam->setFrameSize(width, height);
    
    if (webcam->initialize()) {
        std::cout << "✅ Created webcam capture successfully" << std::endl;
        return std::unique_ptr<IWebcamCapture>(webcam.release());
    } else {
        std::cerr << "❌ Failed to initialize webcam capture" << std::endl;
        return std::unique_ptr<IWebcamCapture>();
    }
}
