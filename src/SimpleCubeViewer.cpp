#include "SimpleCubeViewer.h"
#include <iostream>
#include <cmath>

#ifdef __APPLE__
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
#else
    #include <GL/gl.h>
    #include <GL/glu.h>
#endif

SimpleCubeViewer::SimpleCubeViewer() 
    : cameraDistance(5.0f)
    , cameraTheta(0.0f)
    , cameraPhi(0.0f)
    , lastMouseX(0.0)
    , lastMouseY(0.0)
    , firstMouse(true)
    , windowWidth(800)
    , windowHeight(600)
    , cubeRotation(0.0f)
{
}

SimpleCubeViewer::~SimpleCubeViewer() {
}

bool SimpleCubeViewer::initialize(GLFWwindow* window) {
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    
    // Enable back face culling for better performance
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    // Set clear color to a nice dark blue
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    
    // Get initial window size
    glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
    handleResize(windowWidth, windowHeight);
    
    std::cout << "SimpleCubeViewer initialized successfully!" << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  Click and drag - Orbit around cube" << std::endl;
    std::cout << "  ESC - Exit" << std::endl;
    
    return true;
}

void SimpleCubeViewer::handleResize(int width, int height) {
    windowWidth = width;
    windowHeight = height;
    
    glViewport(0, 0, width, height);
    
    // Set up perspective projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    float aspect = (float)width / (float)height;
    gluPerspective(45.0, aspect, 0.1, 100.0);
}

void SimpleCubeViewer::handleMouseInput(double xpos, double ypos, bool isDragging) {
    if (firstMouse) {
        lastMouseX = xpos;
        lastMouseY = ypos;
        firstMouse = false;
        return;
    }
    
    if (isDragging) {
        double deltaX = xpos - lastMouseX;
        double deltaY = ypos - lastMouseY;
        
        // Update camera angles based on mouse movement
        cameraTheta += deltaX * 0.01f;  // horizontal rotation
        cameraPhi += deltaY * 0.01f;    // vertical rotation
        
        // Clamp vertical angle to avoid flipping
        const float maxPhi = M_PI * 0.48f;  // Just under 90 degrees
        if (cameraPhi > maxPhi) cameraPhi = maxPhi;
        if (cameraPhi < -maxPhi) cameraPhi = -maxPhi;
    }
    
    lastMouseX = xpos;
    lastMouseY = ypos;
}

void SimpleCubeViewer::updateCamera() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Calculate camera position using spherical coordinates
    float x = cameraDistance * cos(cameraPhi) * cos(cameraTheta);
    float y = cameraDistance * sin(cameraPhi);
    float z = cameraDistance * cos(cameraPhi) * sin(cameraTheta);
    
    // Look at the center of the scene
    gluLookAt(x, y, z,     // camera position
              0, 0, 0,     // look at center
              0, 1, 0);    // up vector
}

void SimpleCubeViewer::renderCube() {
    // Apply a slow rotation to make the cube more interesting
    cubeRotation += 0.5f;
    if (cubeRotation > 360.0f) cubeRotation -= 360.0f;
    
    glPushMatrix();
    glRotatef(cubeRotation, 0.1f, 1.0f, 0.1f);  // Rotate on a slight diagonal
    
    // Draw a colorful cube
    glBegin(GL_QUADS);
    
    // Front face (red)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    
    // Back face (green)
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    
    // Top face (blue)
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    
    // Bottom face (yellow)
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    
    // Right face (magenta)
    glColor3f(1.0f, 0.0f, 1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    
    // Left face (cyan)
    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    
    glEnd();
    glPopMatrix();
}

void SimpleCubeViewer::render() {
    // Clear the screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Update camera position
    updateCamera();
    
    // Render the cube
    renderCube();
}
