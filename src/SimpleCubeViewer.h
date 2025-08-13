#pragma once

#include <GLFW/glfw3.h>
#include <opencv2/opencv.hpp>
#include "IWebcamCapture.h"
#include "IDepthEstimator.h"
#include <memory>

class SimpleCubeViewer {
public:
    SimpleCubeViewer();
    ~SimpleCubeViewer();
    
    bool initialize(GLFWwindow* window);
    void render();
    void handleMouseInput(double xpos, double ypos, bool isDragging);
    void handleResize(int width, int height);
    
private:
    void setupCube();
    void renderCube();
    void updateCamera();
    void initializeWebcam();
    void initializeDepthEstimator();
    void updateDepthTexture();
    void createTexture();
    
    // Camera orbit controls
    float cameraDistance;
    float cameraTheta;    // horizontal angle
    float cameraPhi;      // vertical angle
    
    // Mouse input tracking
    double lastMouseX;
    double lastMouseY;
    bool firstMouse;
    
    // Window dimensions
    int windowWidth;
    int windowHeight;
    
    // Rotation for the cube
    float cubeRotation;
    
    // Webcam and texture
    std::unique_ptr<IWebcamCapture> webcam;
    std::unique_ptr<IDepthEstimator> depthEstimator;
    cv::Mat webcamFrame;
    cv::Mat depthFrame;
    GLuint textureID;
    bool webcamActive;
    bool depthEstimatorActive;
};
