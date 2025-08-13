#include <GLFW/glfw3.h>
#include <iostream>
#include "SimpleCubeViewer.h"

// Global variables
SimpleCubeViewer* cubeViewer = nullptr;
bool mousePressed = false;

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

// Mouse button callback
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            mousePressed = true;
        } else if (action == GLFW_RELEASE) {
            mousePressed = false;
        }
    }
}

// Mouse position callback
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if (cubeViewer) {
        cubeViewer->handleMouseInput(xpos, ypos, mousePressed);
    }
}

// Window resize callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    if (cubeViewer) {
        cubeViewer->handleResize(width, height);
    }
}

int main() {
    std::cout << "=== Simple 3D Cube Demo ===" << std::endl;
    std::cout << "Initializing 3D cube viewer..." << std::endl;
    
    // Set error callback
    glfwSetErrorCallback(error_callback);
    
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    
    // Create window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Simple 3D Cube Demo - Click and Drag to Orbit", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    // Set up OpenGL context
    glfwMakeContextCurrent(window);
    
    // Set callbacks
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    // Enable V-Sync
    glfwSwapInterval(1);
    
    // Initialize cube viewer
    cubeViewer = new SimpleCubeViewer();
    if (!cubeViewer->initialize(window)) {
        std::cerr << "Failed to initialize cube viewer" << std::endl;
        delete cubeViewer;
        glfwTerminate();
        return -1;
    }
    
    std::cout << "🎮 3D Cube Demo is ready!" << std::endl;
    
    // Main render loop
    while (!glfwWindowShouldClose(window)) {
        // Render the scene
        cubeViewer->render();
        
        // Swap front and back buffers
        glfwSwapBuffers(window);
        
        // Poll for and process events
        glfwPollEvents();
    }
    
    std::cout << "Closing 3D cube demo..." << std::endl;
    
    // Clean up
    delete cubeViewer;
    glfwTerminate();
    
    std::cout << "✅ 3D Cube Demo completed successfully!" << std::endl;
    return 0;
}
