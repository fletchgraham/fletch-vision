#include "SimpleCubeViewer.h"
#include "WebcamFactory.h"
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
    , textureID(0)
    , webcamActive(false)
{
}

SimpleCubeViewer::~SimpleCubeViewer() {
    // Clean up texture
    if (textureID != 0) {
        glDeleteTextures(1, &textureID);
    }
    
    // Clean up webcam
    if (webcam) {
        webcam->release();
    }
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
    
    // Initialize webcam and texture
    initializeWebcam();
    createTexture();
    
    std::cout << "SimpleCubeViewer initialized successfully!" << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  Click and drag - Orbit around cube" << std::endl;
    std::cout << "  ESC - Exit" << std::endl;
    
    if (webcamActive) {
        std::cout << "📹 Webcam active - cube faces will show live camera feed!" << std::endl;
    } else {
        std::cout << "📷 No webcam - cube will show solid colors" << std::endl;
    }
    
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
    
    // Enable texturing if webcam is active
    if (webcamActive && textureID != 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glColor3f(1.0f, 1.0f, 1.0f);  // White color to show texture properly
    }
    
    // Draw a textured cube
    glBegin(GL_QUADS);
    
    // Front face
    if (webcamActive) {
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
    } else {
        glColor3f(1.0f, 0.0f, 0.0f);  // Red fallback
        glVertex3f(-1.0f, -1.0f,  1.0f);
        glVertex3f( 1.0f, -1.0f,  1.0f);
        glVertex3f( 1.0f,  1.0f,  1.0f);
        glVertex3f(-1.0f,  1.0f,  1.0f);
    }
    
    // Back face
    if (webcamActive) {
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
    } else {
        glColor3f(0.0f, 1.0f, 0.0f);  // Green fallback
        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f(-1.0f,  1.0f, -1.0f);
        glVertex3f( 1.0f,  1.0f, -1.0f);
        glVertex3f( 1.0f, -1.0f, -1.0f);
    }
    
    // Top face
    if (webcamActive) {
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
    } else {
        glColor3f(0.0f, 0.0f, 1.0f);  // Blue fallback
        glVertex3f(-1.0f,  1.0f, -1.0f);
        glVertex3f(-1.0f,  1.0f,  1.0f);
        glVertex3f( 1.0f,  1.0f,  1.0f);
        glVertex3f( 1.0f,  1.0f, -1.0f);
    }
    
    // Bottom face
    if (webcamActive) {
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
    } else {
        glColor3f(1.0f, 1.0f, 0.0f);  // Yellow fallback
        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f( 1.0f, -1.0f, -1.0f);
        glVertex3f( 1.0f, -1.0f,  1.0f);
        glVertex3f(-1.0f, -1.0f,  1.0f);
    }
    
    // Right face
    if (webcamActive) {
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
    } else {
        glColor3f(1.0f, 0.0f, 1.0f);  // Magenta fallback
        glVertex3f( 1.0f, -1.0f, -1.0f);
        glVertex3f( 1.0f,  1.0f, -1.0f);
        glVertex3f( 1.0f,  1.0f,  1.0f);
        glVertex3f( 1.0f, -1.0f,  1.0f);
    }
    
    // Left face
    if (webcamActive) {
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    } else {
        glColor3f(0.0f, 1.0f, 1.0f);  // Cyan fallback
        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f(-1.0f, -1.0f,  1.0f);
        glVertex3f(-1.0f,  1.0f,  1.0f);
        glVertex3f(-1.0f,  1.0f, -1.0f);
    }
    
    glEnd();
    
    // Disable texturing
    if (webcamActive && textureID != 0) {
        glDisable(GL_TEXTURE_2D);
    }
    
    glPopMatrix();
}

void SimpleCubeViewer::initializeWebcam() {
    std::cout << "Initializing webcam for 3D cube texturing..." << std::endl;
    
    // Create webcam using the factory - demonstration of easy integration!
    webcam = WebcamFactory::create();
    
    if (webcam && webcam->isActive()) {
        webcamActive = true;
        cv::Size frameSize = webcam->getFrameSize();
        std::cout << "✅ Webcam initialized successfully for 3D demo!" << std::endl;
        std::cout << "Frame size: " << frameSize.width << "x" << frameSize.height << std::endl;
    } else {
        webcamActive = false;
        std::cout << "⚠️  No webcam available - cube will use solid colors" << std::endl;
    }
}

void SimpleCubeViewer::createTexture() {
    // Generate OpenGL texture
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void SimpleCubeViewer::updateWebcamTexture() {
    if (!webcam || !webcam->isActive() || textureID == 0) {
        return;
    }
    
    // Capture frame from webcam - just one line thanks to the refactored interface!
    if (webcam->captureFrame(webcamFrame) && !webcamFrame.empty()) {
        // Convert BGR to RGB for OpenGL
        cv::Mat rgbFrame;
        cv::cvtColor(webcamFrame, rgbFrame, cv::COLOR_BGR2RGB);
        
        // Flip vertically for OpenGL texture coordinates
        cv::Mat flippedFrame;
        cv::flip(rgbFrame, flippedFrame, 0);
        
        // Update OpenGL texture
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 
                     flippedFrame.cols, flippedFrame.rows, 0, 
                     GL_RGB, GL_UNSIGNED_BYTE, flippedFrame.data);
    }
}

void SimpleCubeViewer::render() {
    // Update webcam texture if available
    if (webcamActive) {
        updateWebcamTexture();
    }
    
    // Clear the screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Update camera position
    updateCamera();
    
    // Render the cube
    renderCube();
}
