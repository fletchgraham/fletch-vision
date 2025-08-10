# Fletch Vision

A C++ computer vision project with multiple demos using OpenCV, GLFW, and OpenGL.

## Prerequisites

- OpenCV 4.x
- GLFW 3.x
- CMake 3.10+ (for CMake build)
- pkg-config (for Makefile build)
- **Camera permissions** (macOS will prompt when first running)

### Installing Dependencies on macOS

```bash
# Using Homebrew
brew install opencv glfw

# Accept Xcode license if needed
sudo xcodebuild -license accept
```

## Camera Permissions

On macOS, the app will request camera access when first launched. You can also manually grant permissions:

1. Go to **System Preferences/Settings > Privacy & Security > Camera**
2. Find your terminal application and enable camera access
3. Or run the app and click "OK" when the permission dialog appears

## Building and Running

### Option 1: Using Makefile (Recommended)

```bash
make clean && make
./fletch_vision
```

### Option 2: Using CMake

```bash
mkdir -p build
cd build
cmake ..
make
./bin/fletch_vision
```

### Testing Camera Connection

```bash
# Build and run a simple camera test
make test
./camera_test
```

## Features

- **Live webcam feed** with OpenGL rendering
- **Canny edge detection** filter (toggle with E key)
- **Real-time face detection** with bounding boxes (toggle with F key)
- **Camera auto-detection** with multiple backend support
- **Cross-platform compatibility** (macOS, Linux)

## Controls

- **ESC key** - Quit the application
- **E key** - Toggle Canny edge detection filter
- **F key** - Toggle face detection with bounding boxes
- Both features can be used simultaneously
