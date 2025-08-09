#include <GLFW/glfw3.h>
#include <iostream>

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

int main() {
    std::cout << "=== Webcam CV Demo ===" << std::endl;
    std::cout << "Initializing window..." << std::endl;
    
    // Set error callback
    glfwSetErrorCallback(error_callback);
    
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    
    // Create window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Webcam CV Demo", NULL, NULL);
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
    
    std::cout << "Window created! Press ESC to close." << std::endl;
    
    // Main render loop
    while (!glfwWindowShouldClose(window)) {
        // Set background color (purple/blue gradient effect)
        glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Swap front and back buffers
        glfwSwapBuffers(window);
        
        // Poll for and process events
        glfwPollEvents();
    }
    
    std::cout << "Closing window..." << std::endl;
    
    // Clean up
    glfwTerminate();
    
    std::cout << "✅ Demo completed successfully!" << std::endl;
    std::cout << "Next: Add webcam feed to the window" << std::endl;
    return 0;
}
