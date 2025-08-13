#pragma once

#include <GLFW/glfw3.h>

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
};
