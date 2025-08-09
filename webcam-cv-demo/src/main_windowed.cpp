#include <GLFW/glfw3.h>
#include <opencv2/opencv.hpp>
#include <iostream>

// Global variables
cv::VideoCapture cap;
cv::Mat frame;
GLuint textureID;
bool webcamActive = false;

// Error callback function
void error_callback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

// Key callback function
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

// Initialize webcam
bool initWebcam() {
    std::cout << "Attempting to open camera..." << std::endl;
    
    // Try different camera backends and indices
    std::vector<int> backends = {cv::CAP_AVFOUNDATION, cv::CAP_ANY};
    std::vector<int> indices = {0, 1, 2};
    
    for (int backend : backends) {
        for (int index : indices) {
            std::cout << "Trying camera index " << index << " with backend " << backend << std::endl;
            cap.open(index, backend);
            
            if (cap.isOpened()) {
                std::cout << "Camera opened successfully with index " << index << " and backend " << backend << std::endl;
                
                // Set camera properties
                cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
                cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
                
                // Test if we can actually read a frame
                cv::Mat testFrame;
                if (cap.read(testFrame) && !testFrame.empty()) {
                    std::cout << "✅ Webcam initialized successfully!" << std::endl;
                    std::cout << "Frame size: " << testFrame.cols << "x" << testFrame.rows << std::endl;
                    return true;
                } else {
                    std::cout << "Camera opened but couldn't read frame" << std::endl;
                    cap.release();
                }
            }
        }
    }
    
    std::cerr << "❌ Error: Could not open any webcam" << std::endl;
    std::cerr << "Make sure:" << std::endl;
    std::cerr << "1. Camera permissions are granted to Terminal" << std::endl;
    std::cerr << "2. No other app is using the camera" << std::endl;
    std::cerr << "3. Camera is properly connected" << std::endl;
    return false;
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
    if (!webcamActive) return;
    
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
    webcamActive = initWebcam();
    
    if (webcamActive) {
        std::cout << "Live webcam feed active! Press ESC to close." << std::endl;
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
        if (webcamActive) {
            // Capture frame from webcam
            cap >> frame;
            if (!frame.empty()) {
                matToTexture(frame);
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
    if (webcamActive) {
        cap.release();
    }
    glDeleteTextures(1, &textureID);
    glfwTerminate();
    
    std::cout << "✅ Demo completed successfully!" << std::endl;
    std::cout << "Next: Add computer vision effects and filters" << std::endl;
    return 0;
}
