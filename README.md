# Fletch Vision

A C++ computer vision project with multiple demos and applications using OpenCV, GLFW, and OpenGL.

## Project Structure

```
fletch-vision/
├── src/               # Source code for all demos
├── include/           # Header files
├── obj/               # Object files (build artifacts)
├── bin/               # Compiled binaries
├── fletchBricks/      # Breakout game demo
├── Makefile           # Build configuration
└── CMakeLists.txt     # Alternative CMake build
```

## Current Demos

### Webcam CV Demo

A live webcam application with computer vision effects:

- Live video capture and display
- Real-time edge detection (press 'E' to toggle)
- OpenGL rendering for smooth playback
- Graceful fallback when camera unavailable

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
./webcam_cv_demo
```

### Option 2: Using CMake

```bash
mkdir -p build
cd build
cmake ..
make
./bin/webcam_cv_demo
```

### Testing Camera Connection

```bash
# Build and run a simple camera test
make test
./camera_test
```

## Controls

- **ESC key** - Quit the application
- **E key** - Toggle edge detection filter
- The window shows live webcam feed if camera is available
- Falls back to colored background if camera access is denied

## Features

- ✅ Live webcam capture using OpenCV
- ✅ Real-time video display using OpenGL
- ✅ Cross-platform window management with GLFW
- ✅ Graceful fallback when camera is unavailable
- ✅ Proper resource cleanup
- ✅ Real-time edge detection filter

## Upcoming Features

This foundation supports adding:

- Multiple computer vision filters in one app
- Object detection
- Face recognition
- Motion tracking
- Interactive demos selection
- Real-time parameter adjustment
