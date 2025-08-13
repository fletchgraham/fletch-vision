#pragma once

#include "IWebcamCapture.h"
#include <memory>

/**
 * Factory for creating webcam capture instances.
 * This makes it easy for any demo to get webcam functionality.
 */
class WebcamFactory {
public:
    /**
     * Create a webcam capture instance.
     * @return Unique pointer to the created webcam capture, or nullptr if creation failed
     */
    static std::unique_ptr<IWebcamCapture> create();
    
    /**
     * Create a webcam capture instance with custom frame size.
     * @param width Preferred frame width
     * @param height Preferred frame height
     * @return Unique pointer to the created webcam capture, or nullptr if creation failed
     */
    static std::unique_ptr<IWebcamCapture> create(int width, int height);
};
