#include "SimpleCubeViewer.h"
#include "WebcamFactory.h"
#include "DepthEstimatorFactory.h"
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
    , textureID(0)
    , webcamActive(false)
    , depthEstimatorActive(false)
    , vertices(nullptr)
    , texCoords(nullptr)
    , indices(nullptr)
{
}

SimpleCubeViewer::~SimpleCubeViewer() {
    // Clean up texture
    if (textureID != 0) {
        glDeleteTextures(1, &textureID);
    }
    
    // Clean up mesh data
    if (vertices) delete[] vertices;
    if (texCoords) delete[] texCoords;
    if (indices) delete[] indices;
    
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
    
    // Initialize webcam and depth estimator
    initializeWebcam();
    initializeDepthEstimator();
    
    // Setup mesh and texture
    setupMesh();
    createTexture();
    
    std::cout << "SimpleCubeViewer initialized successfully!" << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  Click and drag - Orbit around mesh" << std::endl;
    std::cout << "  ESC - Exit" << std::endl;
    
    if (webcamActive && depthEstimatorActive) {
        std::cout << "🔥 Depth-displaced mesh active - 3D surface with webcam texture!" << std::endl;
    } else if (webcamActive) {
        std::cout << "📹 Webcam active - flat mesh will show live camera feed!" << std::endl;
    } else {
        std::cout << "📷 No webcam - mesh will show solid colors" << std::endl;
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

void SimpleCubeViewer::setupMesh() {
    // Allocate memory for mesh data
    vertices = new float[MESH_VERTICES * 3];  // x, y, z for each vertex
    texCoords = new float[MESH_VERTICES * 2]; // u, v for each vertex
    indices = new unsigned int[MESH_INDICES]; // triangle indices
    
    // Generate vertices, texture coordinates, and indices for 128x128 grid
    for (int y = 0; y < MESH_HEIGHT; y++) {
        for (int x = 0; x < MESH_WIDTH; x++) {
            int index = y * MESH_WIDTH + x;
            
            // Vertices: map from 0 to 1, then center around origin (-1 to 1)
            vertices[index * 3 + 0] = (float)x / (MESH_WIDTH - 1) * 2.0f - 1.0f;   // X: -1 to 1
            vertices[index * 3 + 1] = (float)y / (MESH_HEIGHT - 1) * 2.0f - 1.0f;  // Y: -1 to 1
            vertices[index * 3 + 2] = 0.0f;  // Z: initially flat, will be displaced by depth
            
            // Texture coordinates: map directly to webcam frame
            texCoords[index * 2 + 0] = (float)x / (MESH_WIDTH - 1);   // U: 0 to 1
            texCoords[index * 2 + 1] = (float)y / (MESH_HEIGHT - 1);  // V: 0 to 1
        }
    }
    
    // Generate indices for triangles (each quad becomes two triangles)
    int currentIndex = 0;
    for (int y = 0; y < MESH_HEIGHT - 1; y++) {
        for (int x = 0; x < MESH_WIDTH - 1; x++) {
            // Bottom left of current quad
            int bottomLeft = y * MESH_WIDTH + x;
            int bottomRight = bottomLeft + 1;
            int topLeft = (y + 1) * MESH_WIDTH + x;
            int topRight = topLeft + 1;
            
            // First triangle: bottom-left, bottom-right, top-left
            indices[currentIndex++] = bottomLeft;
            indices[currentIndex++] = bottomRight;
            indices[currentIndex++] = topLeft;
            
            // Second triangle: bottom-right, top-right, top-left
            indices[currentIndex++] = bottomRight;
            indices[currentIndex++] = topRight;
            indices[currentIndex++] = topLeft;
        }
    }
}

void SimpleCubeViewer::renderMesh() {
    glPushMatrix();
    
    // Scale and position the mesh
    glScalef(2.0f, 1.5f, 1.0f);  // Make it wider and slightly taller
    
    // Enable texturing with the webcam feed
    if (webcamActive && textureID != 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glColor3f(1.0f, 1.0f, 1.0f);  // White to show texture properly
    }
    
    // Draw the mesh using triangles
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < MESH_INDICES; i += 3) {
        for (int j = 0; j < 3; j++) {
            int vertexIndex = indices[i + j];
            
            // Set texture coordinate
            glTexCoord2f(texCoords[vertexIndex * 2], texCoords[vertexIndex * 2 + 1]);
            
            // Set vertex position
            glVertex3f(vertices[vertexIndex * 3], vertices[vertexIndex * 3 + 1], vertices[vertexIndex * 3 + 2]);
        }
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

void SimpleCubeViewer::initializeDepthEstimator() {
    std::cout << "Initializing depth estimator for inferno depth mapping..." << std::endl;
    
    // Create depth estimator using the factory with model path
    depthEstimator = DepthEstimatorFactory::create("models/midasv2_small_256x256.onnx");
    
    if (depthEstimator) {
        depthEstimatorActive = true;
        std::cout << "✅ Depth estimator initialized successfully for 3D demo!" << std::endl;
        std::cout << "🔥 Inferno depth mapping will be applied to cube faces!" << std::endl;
    } else {
        depthEstimatorActive = false;
        std::cout << "⚠️  Depth estimator not available - falling back to webcam or solid colors" << std::endl;
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

void SimpleCubeViewer::updateMeshTexture() {
    if (!webcam || !webcam->isActive() || textureID == 0) {
        return;
    }
    
    // Capture frame from webcam for texture
    if (webcam->captureFrame(webcamFrame) && !webcamFrame.empty()) {
        // Convert BGR to RGB for OpenGL
        cv::Mat rgbFrame;
        cv::cvtColor(webcamFrame, rgbFrame, cv::COLOR_BGR2RGB);
        
        // Flip vertically for OpenGL texture coordinates
        cv::Mat flippedFrame;
        cv::flip(rgbFrame, flippedFrame, 0);
        
        // Update OpenGL texture with webcam frame
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 
                     flippedFrame.cols, flippedFrame.rows, 0, 
                     GL_RGB, GL_UNSIGNED_BYTE, flippedFrame.data);
    }
}

void SimpleCubeViewer::updateMeshGeometry() {
    if (!depthEstimator || !depthEstimatorActive || !webcam) return;
    
    // Capture current frame for depth estimation
    cv::Mat currentFrame;
    if (!webcam->captureFrame(currentFrame) || currentFrame.empty()) return;
    
    cv::Mat depthMap = depthEstimator->estimateDepth(currentFrame);
    if (depthMap.empty()) return;
    
    // Resize depth map to match mesh resolution
    cv::Mat resizedDepthMap;
    cv::resize(depthMap, resizedDepthMap, cv::Size(MESH_WIDTH, MESH_HEIGHT));
    
    // Convert to single channel if needed (depth should be grayscale)
    cv::Mat grayDepth;
    if (resizedDepthMap.channels() == 3) {
        cv::cvtColor(resizedDepthMap, grayDepth, cv::COLOR_BGR2GRAY);
    } else {
        grayDepth = resizedDepthMap;
    }
    
    // Update vertex Z coordinates based on depth values
    float depthScale = 0.0005f;  // Scale factor for depth displacement (subtle movement)
    for (int y = 0; y < MESH_HEIGHT; y++) {
        for (int x = 0; x < MESH_WIDTH; x++) {
            int index = y * MESH_WIDTH + x;
            
            // Flip y-coordinate to match texture coordinate system
            int flippedY = (MESH_HEIGHT - 1) - y;
            
            // Get depth value - handle different data types
            float depth = 0.0f;
            if (grayDepth.type() == CV_32F) {
                depth = grayDepth.at<float>(flippedY, x);
            } else if (grayDepth.type() == CV_8U) {
                depth = grayDepth.at<uchar>(flippedY, x) / 255.0f;
            } else if (grayDepth.type() == CV_32FC3) {
                // If it's still 3-channel, get the first channel
                cv::Vec3f pixel = grayDepth.at<cv::Vec3f>(flippedY, x);
                depth = pixel[0];
            } else {
                // Convert to float if unknown type
                grayDepth.convertTo(grayDepth, CV_32F);
                depth = grayDepth.at<float>(flippedY, x);
            }
            
            // Apply depth displacement to Z coordinate
            vertices[index * 3 + 2] = depth * depthScale;
        }
    }
}

void SimpleCubeViewer::render() {
    // Update mesh texture and geometry if both webcam and depth estimator are available
    if (webcamActive && depthEstimatorActive) {
        updateMeshTexture();
        updateMeshGeometry();
    }
    
    // Clear the screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Update camera position
    updateCamera();
    
    // Render the mesh
    renderMesh();
}
