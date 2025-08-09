# Webcam CV Demo

A C++ computer vision demo application that displays live webcam feed in a window using OpenCV and GLFW.

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

### Option 1: Using Makefile

```bash
make clean && make
./webcam_cv_demo
```

### Option 2: Using CMake (Alternative)

```bash
mkdir build
cd build
cmake ..
make
./bin/webcam_cv_demo
```

## Controls

- **ESC key** - Quit the application
- The window shows live webcam feed if camera is available
- Falls back to colored background if camera access is denied

## Features

- ✅ Live webcam capture using OpenCV
- ✅ Real-time video display using OpenGL
- ✅ Cross-platform window management with GLFW
- ✅ Graceful fallback when camera is unavailable
- ✅ Proper resource cleanup

## Next Steps

This foundation supports adding:
- Real-time image filters
- Object detection
- Face recognition
- Motion tracking
- Computer vision effects
