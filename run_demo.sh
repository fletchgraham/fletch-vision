#!/bin/bash

# Fletch Vision Demo Script
# Demonstrates the depth estimation capabilities

echo "🎥 Fletch Vision - Depth Estimation Demo"
echo "========================================="

# Check if models exist
if [ ! -f "models/model-small.onnx" ] && [ ! -f "models/model-f6b98070.onnx" ]; then
    echo "❌ No MiDaS models found!"
    echo "Please run: ./download_models.sh first"
    exit 1
fi

echo "✅ MiDaS models found!"
echo ""
echo "🎮 Controls:"
echo "   ESC - Exit application"
echo "   E   - Toggle edge detection"
echo "   F   - Toggle face detection"
echo "   D   - Toggle depth estimation heat map"
echo ""
echo "🚀 Starting application..."
echo ""

# Build and run
make clean && make && ./fletch_vision
