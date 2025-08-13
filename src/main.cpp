#include <GLFW/glfw3.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "DepthEstimatorFactory.h"
#include "WebcamFactory.h"
#include <memory>

// Global variables
std::unique_ptr<IWebcamCapture> webcam;
cv::Mat frame;
GLuint textureID;

// Simple edge detection toggle
bool edgeDetectionEnabled = false;

// Face detection toggle and classifier
bool faceDetectionEnabled = false;
cv::CascadeClassifier faceCascade;

// Depth estimation toggle and estimator
bool depthEstimationEnabled = false;
std::unique_ptr<IDepthEstimator> depthEstimator;

// Error callback function
void error_callback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

// Key callback function
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        edgeDetectionEnabled = !edgeDetectionEnabled;
        std::cout << "Edge detection: " << (edgeDetectionEnabled ? "ON" : "OFF") << std::endl;
    }
    if (key == GLFW_KEY_F && action == GLFW_PRESS) {
        faceDetectionEnabled = !faceDetectionEnabled;
        std::cout << "Face detection: " << (faceDetectionEnabled ? "ON" : "OFF") << std::endl;
    }
    if (key == GLFW_KEY_D && action == GLFW_PRESS) {
        depthEstimationEnabled = !depthEstimationEnabled;
        std::cout << "Depth estimation: " << (depthEstimationEnabled ? "ON" : "OFF") << std::endl;
    }
}

// Initialize webcam
bool initWebcam() {
    // Create webcam using the factory
    webcam = WebcamFactory::create();
    
    if (webcam && webcam->isActive()) {
        std::cout << "✅ Webcam initialized successfully!" << std::endl;
        return true;
    } else {
        std::cerr << "❌ Error: Could not initialize webcam" << std::endl;
        return false;
    }
}

// Initialize face detection
bool initFaceDetection() {
    // Try to load the frontal face cascade classifier
    std::vector<std::string> cascadePaths = {
        "/opt/homebrew/share/opencv4/haarcascades/haarcascade_frontalface_alt.xml",
        "/usr/local/share/opencv4/haarcascades/haarcascade_frontalface_alt.xml",
        "/usr/share/opencv4/haarcascades/haarcascade_frontalface_alt.xml",
        "haarcascade_frontalface_alt.xml"
    };
    
    for (const std::string& path : cascadePaths) {
        if (faceCascade.load(path)) {
            std::cout << "✅ Face cascade loaded from: " << path << std::endl;
            return true;
        }
    }
    
    std::cerr << "❌ Error: Could not load face cascade classifier" << std::endl;
    std::cerr << "Make sure OpenCV haarcascades are installed" << std::endl;
    return false;
}

// Initialize depth estimation
bool initDepthEstimation() {
    // Create depth estimator using the factory with default paths
    depthEstimator = DepthEstimatorFactory::createWithDefaultPaths();
    
    if (depthEstimator) {
        std::cout << "✅ Depth estimation initialized successfully" << std::endl;
        return true;
    } else {
        std::cerr << "❌ Error: Could not initialize depth estimation" << std::endl;
        return false;
    }
}

// Process frame with edge detection, face detection, and/or depth estimation
cv::Mat processFrame(const cv::Mat& inputFrame) {
    if (inputFrame.empty()) {
        return inputFrame;
    }
    
    cv::Mat result = inputFrame.clone();
    
    // Apply edge detection if enabled
    if (edgeDetectionEnabled) {
        cv::Mat gray, edges;
        cv::cvtColor(inputFrame, gray, cv::COLOR_BGR2GRAY);
        cv::Canny(gray, edges, 50, 150);
        cv::cvtColor(edges, result, cv::COLOR_GRAY2BGR);
    }
    
    // Apply depth estimation if enabled
    if (depthEstimationEnabled && depthEstimator && depthEstimator->isInitialized()) {
        cv::Mat depthMap = depthEstimator->estimateDepth(inputFrame);
        if (!depthMap.empty()) {
            // Overlay depth heat map on the current result
            result = depthEstimator->overlayDepthHeatMap(result, depthMap, 0.9f);
        }
    }
    
    // Apply face detection if enabled
    if (faceDetectionEnabled && !faceCascade.empty()) {
        cv::Mat gray;
        cv::cvtColor(inputFrame, gray, cv::COLOR_BGR2GRAY);
        
        std::vector<cv::Rect> faces;
        faceCascade.detectMultiScale(gray, faces, 1.1, 3, 0, cv::Size(30, 30));
        
        // Draw bounding boxes around detected faces
        for (const cv::Rect& face : faces) {
            cv::rectangle(result, face, cv::Scalar(0, 255, 0), 2);
            
            // Add a label
            std::string label = "Face";
            int baseline;
            cv::Size labelSize = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseline);
            cv::Point labelPos(face.x, face.y - 10);
            
            // Ensure label is within frame bounds
            if (labelPos.y < 0) labelPos.y = face.y + labelSize.height + 10;
            
            cv::putText(result, label, labelPos, cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 1);
        }
        
        // Print number of faces detected
        static int frameCount = 0;
        frameCount++;
        if (frameCount % 30 == 0) { // Print every 30 frames to avoid spam
            std::cout << "Detected " << faces.size() << " face(s)" << std::endl;
        }
    }
    
    return result;
}

// Convert OpenCV Mat to OpenGL texture
void matToTexture(const cv::Mat& mat) {
    if (mat.empty()) return;
    
    // Convert BGR to RGB
    cv::Mat rgbMat;
    cv::cvtColor(mat, rgbMat, cv::COLOR_BGR2RGB);
    
    // Flip vertically for OpenGL
    cv::flip(rgbMat, rgbMat, 0);
    
    // Update texture
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, rgbMat.cols, rgbMat.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, rgbMat.data);
}

// Render the texture
void renderTexture(int windowWidth, int windowHeight) {
    if (!webcam || !webcam->isActive()) return;
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    // Set up orthographic projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, windowWidth, 0, windowHeight, -1, 1);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Draw texture as quad
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(0, 0);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(windowWidth, 0);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(windowWidth, windowHeight);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(0, windowHeight);
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
}

int main() {
    std::cout << "=== Webcam CV Demo ===" << std::endl;
    std::cout << "Initializing window and webcam..." << std::endl;
    
    // Set error callback
    glfwSetErrorCallback(error_callback);
    
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    
    // Create window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Webcam CV Demo - Live Feed", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    // Set up OpenGL context
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    
    // Enable V-Sync
    glfwSwapInterval(1);
    
    // Initialize OpenGL texture
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Initialize webcam
    bool webcamActive = initWebcam();
    
    // Initialize face detection
    bool faceDetectionAvailable = initFaceDetection();
    
    // Initialize depth estimation
    bool depthEstimationAvailable = initDepthEstimation();
    
    if (webcamActive) {
        std::cout << "🎥 Live webcam feed active! Controls:" << std::endl;
        std::cout << "  ESC - Exit" << std::endl;
        std::cout << "  E   - Toggle edge detection (currently " << (edgeDetectionEnabled ? "ON" : "OFF") << ")" << std::endl;
        if (faceDetectionAvailable) {
            std::cout << "  F   - Toggle face detection (currently " << (faceDetectionEnabled ? "ON" : "OFF") << ")" << std::endl;
        } else {
            std::cout << "  F   - Face detection (unavailable - cascade not loaded)" << std::endl;
        }
        if (depthEstimationAvailable) {
            std::cout << "  D   - Toggle depth estimation heat map (currently " << (depthEstimationEnabled ? "ON" : "OFF") << ")" << std::endl;
        } else {
            std::cout << "  D   - Depth estimation (unavailable - model not loaded)" << std::endl;
        }
    } else {
        std::cout << "Webcam failed to initialize. Showing colored background. Press ESC to close." << std::endl;
    }
    
    // Main render loop
    while (!glfwWindowShouldClose(window)) {
        // Get window size
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        
        // Clear the screen
        if (webcam && webcam->isActive()) {
            // Capture frame from webcam
            if (webcam->captureFrame(frame) && !frame.empty()) {
                // Process frame (apply edge detection if enabled)
                cv::Mat processedFrame = processFrame(frame);
                matToTexture(processedFrame);
                glClear(GL_COLOR_BUFFER_BIT);
                renderTexture(width, height);
            }
        } else {
            // Fallback: colored background
            glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
        }
        
        // Swap front and back buffers
        glfwSwapBuffers(window);
        
        // Poll for and process events
        glfwPollEvents();
    }
    
    std::cout << "Closing webcam and window..." << std::endl;
    
    // Clean up
    if (webcam) {
        webcam->release();
    }
    glDeleteTextures(1, &textureID);
    glfwTerminate();
    
    std::cout << "✅ Demo completed successfully!" << std::endl;
    std::cout << "Next: Add computer vision effects and filters" << std::endl;
    return 0;
}
