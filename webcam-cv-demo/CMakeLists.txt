cmake_minimum_required(VERSION 3.10)
project(WebcamCVDemo)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenCV
find_package(OpenCV REQUIRED)

# Include directories
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(include)

# Add executable
add_executable(webcam_cv_demo src/main.cpp)

# Link libraries
target_link_libraries(webcam_cv_demo ${OpenCV_LIBS})

# Set output directory
set_target_properties(webcam_cv_demo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
