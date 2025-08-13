// Example demonstrating how a 3D demo can easily use the webcam
// This shows how the refactored webcam interface makes it reusable

#include <iostream>
#include <opencv2/opencv.hpp>
#include "WebcamFactory.h"

int main() {
    std::cout << "=== 3D Demo with Webcam Example ===" << std::endl;
    
    // Easy to add webcam to any demo - just one line!
    auto webcam = WebcamFactory::create();
    
    if (!webcam || !webcam->isActive()) {
        std::cerr << "Could not create webcam. Make sure camera is available." << std::endl;
        return -1;
    }
    
    std::cout << "✅ Webcam successfully integrated into 3D demo!" << std::endl;
    cv::Size frameSize = webcam->getFrameSize();
    std::cout << "Frame size: " << frameSize.width << "x" << frameSize.height << std::endl;
    
    cv::Mat frame;
    std::cout << "Press 'q' to quit, any other key to capture frame" << std::endl;
    
    while (true) {
        // Easy frame capture - just call the interface
        if (webcam->captureFrame(frame) && !frame.empty()) {
            cv::imshow("3D Demo - Webcam Input", frame);
            
            // In a real 3D demo, you could:
            // - Use the frame as a texture on 3D objects
            // - Do motion detection to control 3D scene
            // - Apply computer vision to interact with 3D world
            
            int key = cv::waitKey(30) & 0xFF;
            if (key == 'q') break;
            
            if (key != 255) {  // Any key pressed
                std::cout << "📸 Frame captured! In a 3D demo, this could:" << std::endl;
                std::cout << "  - Apply to 3D object as texture" << std::endl;
                std::cout << "  - Control camera movement" << std::endl;
                std::cout << "  - Trigger 3D animations" << std::endl;
            }
        }
    }
    
    webcam->release();
    cv::destroyAllWindows();
    
    std::cout << "✅ 3D demo with webcam completed successfully!" << std::endl;
    return 0;
}
