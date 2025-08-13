# Fletch Vision

A C++ computer vision project with multiple demos using OpenCV, GLFW, and OpenGL.

## Demos Available

1. **Main Computer Vision Demo** (`fletch_vision`) - Live webcam feed with edge detection, face detection, and depth estimation
2. **Simple 3D Cube Demo** (`simple_cube_viewer`) - Interactive 3D cube with mouse orbit controls

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
# Build all demos
make clean && make

# Run the computer vision demo
./fletch_vision

# Run the 3D cube demo
./simple_cube_viewer

# Or use convenience commands
make run        # Run computer vision demo
make run-cube   # Run 3D cube demo
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

### Computer Vision Demo (`fletch_vision`)

- **Live webcam feed** with OpenGL rendering
- **Canny edge detection** filter (toggle with E key)
- **Real-time face detection** with bounding boxes (toggle with F key)
- **Monocular depth estimation** with heat map overlay using MiDaS (toggle with D key)
- **Camera auto-detection** with multiple backend support
- **Cross-platform compatibility** (macOS, Linux)

### 3D Cube Demo (`simple_cube_viewer`)

- **Interactive 3D cube** with colorful faces
- **Mouse orbit controls** - click and drag to rotate camera around cube
- **Smooth rotation animation** of the cube itself
- **Real-time 3D rendering** using OpenGL
- **No dependencies on OpenCV** - lightweight and fast

## Controls

### Computer Vision Demo

- **ESC key** - Quit the application
- **E key** - Toggle Canny edge detection filter
- **F key** - Toggle face detection with bounding boxes
- **D key** - Toggle depth estimation heat map overlay
- All features can be used simultaneously

### 3D Cube Demo

- **Click and drag** - Orbit camera around the cube
- **ESC key** - Quit the application

## Depth Estimation Setup

The depth estimation feature uses MiDaS neural network models in ONNX format.

### Quick Setup:

1. Download the recommended model:

```bash
curl -L -o models/midasv2_small_256x256.onnx https://github.com/isl-org/MiDaS/releases/download/v2_1/model-small.onnx
```

2. Build and run the application:

```bash
make clean && make
./fletch_vision
```

3. Press **D** to toggle depth estimation

### Model Details:

- **midasv2_small_256x256.onnx** (64MB) - MiDaS v2.1 Small model
- Input resolution: 256×256
- Real-time performance with good accuracy
- Compatible with OpenCV 4.12.0 DNN module

See `models/README.md` for detailed download instructions.
