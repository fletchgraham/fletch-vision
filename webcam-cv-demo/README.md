# Webcam CV Demo

A simple C++ computer vision demo application that displays a window with a colored background as a starting point.

## Prerequisites

- OpenCV 4.x
- CMake 3.10+ (for CMake build)
- pkg-config (for Makefile build)

### Installing OpenCV on macOS

```bash
# Using Homebrew
brew install opencv

# Or using MacPorts
sudo port install opencv4
```

## Building and Running

### Option 1: Using CMake (Recommended)

```bash
mkdir build
cd build
cmake ..
make
./bin/webcam_cv_demo
```

### Option 2: Using Makefile

```bash
make
make run
```

## Controls

- Press 'q' or ESC to quit the application

## Next Steps

This is a basic foundation. Future enhancements could include:
- Live webcam feed
- Object detection
- Face recognition
- Image filters and effects
- Real-time image processing
