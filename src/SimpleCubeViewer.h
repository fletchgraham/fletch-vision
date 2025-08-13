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
    void setupMesh();
    void renderMesh();
    void updateCamera();
    void initializeWebcam();
    void initializeDepthEstimator();
    void updateMeshTexture();
    void createTexture();
    void updateMeshGeometry();
    
    // Mesh properties
    static const int MESH_WIDTH = 128;
    static const int MESH_HEIGHT = 128;
    static const int MESH_VERTICES = MESH_WIDTH * MESH_HEIGHT;
    static const int MESH_QUADS = (MESH_WIDTH - 1) * (MESH_HEIGHT - 1);
    static const int MESH_INDICES = MESH_QUADS * 6; // 2 triangles per quad, 3 vertices per triangle
    
    // Mesh data
    float* vertices;    // x, y, z coordinates
    float* texCoords;   // u, v texture coordinates  
    unsigned int* indices; // triangle indices
    cv::Mat depthMap;   // Current depth map for displacement
    
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
    
    // Webcam and texture
    std::unique_ptr<IWebcamCapture> webcam;
    std::unique_ptr<IDepthEstimator> depthEstimator;
    cv::Mat webcamFrame;
    cv::Mat depthFrame;
    GLuint textureID;
    bool webcamActive;
    bool depthEstimatorActive;
};
