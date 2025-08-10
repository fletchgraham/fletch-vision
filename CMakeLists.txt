cmake_minimum_required(VERSION 3.10)
project(WebcamCVDemo)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenCV
find_package(OpenCV REQUIRED)

# Find GLFW
find_package(glfw3 REQUIRED)

# Include directories
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(include)

# Add executable
add_executable(fletch_vision src/main_windowed.cpp)

# Link libraries
target_link_libraries(fletch_vision ${OpenCV_LIBS} glfw)

# Link macOS frameworks for OpenGL
if(APPLE)
    target_link_libraries(fletch_vision "-framework OpenGL" "-framework Cocoa" "-framework IOKit")
endif()

# Set output directory
set_target_properties(fletch_vision PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
