#include <iostream>
#include <thread>
#include <chrono>

int main() {
    std::cout << "=== Webcam CV Demo ===" << std::endl;
    std::cout << "Basic C++ compilation successful!" << std::endl;
    std::cout << "This confirms our build environment works." << std::endl;
    std::cout << "And this was typed by fletch himself" << std::endl;
    std::cout << std::endl;
    
    // Simple countdown demo
    std::cout << "Running basic demo..." << std::endl;
    for (int i = 3; i > 0; i--) {
        std::cout << "Demo step " << (4-i) << "/3 - Counting: " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    std::cout << std::endl;
    std::cout << "✅ Demo completed successfully!" << std::endl;
    std::cout << "Next: Add graphics library for window creation" << std::endl;
    return 0;
}