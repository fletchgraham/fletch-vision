# Fletch Vision

Real-time computer vision and 3D visualization using C++, OpenCV, and OpenGL. Features monocular depth estimation, 3d mesh displacement based on depth, face tracking, canny edge detection, all realtime on a webcam stream.

## Demos

1. **Computer Vision Demo** (`fletch_vision`) - Live webcam with edge detection, face detection, and depth estimation
2. **3D Mesh Demo** (`simple_cube_viewer`) - 128×128 mesh textured with webcam feed and depth-displaced vertices

## Prerequisites

- OpenCV 4.x with DNN module
- GLFW 3.x
- Camera permissions (macOS will prompt on first run)

## Quick Start

```bash
# Install dependencies (macOS)
brew install opencv glfw

# Build and run
make clean && make
./fletch_vision        # Computer vision demo
./simple_cube_viewer   # 3D mesh demo
```

## Features

### Computer Vision Demo

- Live webcam feed with OpenGL rendering
- **E** - Toggle Canny edge detection
- **F** - Toggle face detection with bounding boxes
- **D** - Toggle MiDaS depth estimation with heat map
- **ESC** - Exit

### 3D Mesh Demo

- Real-time 128×128 quad mesh textured with webcam feed
- Depth-based vertex displacement using MiDaS neural network
- Mouse orbit controls (click and drag)
- **ESC** - Exit

## Depth Estimation Setup

Download the MiDaS model for depth estimation:

```bash
curl -L -o models/midasv2_small_256x256.onnx \
  https://github.com/isl-org/MiDaS/releases/download/v2_1/model-small.onnx
```
