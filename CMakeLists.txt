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

# Add executables
add_executable(fletch_vision 
    src/main.cpp 
    src/DepthEstimator.cpp 
    src/DepthEstimatorFactory.cpp
    src/WebcamCapture.cpp
    src/WebcamFactory.cpp
)
add_executable(simple_cube_viewer 
    src/cube_main.cpp 
    src/SimpleCubeViewer.cpp
    src/WebcamCapture.cpp
    src/WebcamFactory.cpp
)

# Link libraries for main fletch_vision app
target_link_libraries(fletch_vision ${OpenCV_LIBS} glfw)

# Link libraries for simple cube viewer (now with OpenCV for webcam)
target_link_libraries(simple_cube_viewer ${OpenCV_LIBS} glfw)

# Link macOS frameworks for OpenGL
if(APPLE)
    target_link_libraries(fletch_vision "-framework OpenGL" "-framework Cocoa" "-framework IOKit")
    target_link_libraries(simple_cube_viewer "-framework OpenGL" "-framework Cocoa" "-framework IOKit")
endif()

# Set output directory
set_target_properties(fletch_vision PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
set_target_properties(simple_cube_viewer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
