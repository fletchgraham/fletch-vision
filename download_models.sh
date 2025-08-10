#!/bin/bash

# MiDaS Model Download Script
# This script downloads the recommended MiDaS models for depth estimation

echo "🤖 MiDaS Model Download Script"
echo "==============================="

# Create models directory if it doesn't exist
mkdir -p models

# Download the small model (recommended for real-time performance)
echo "📥 Downloading MiDaS small model (21MB) for real-time performance..."
if [ ! -f "models/model-small.onnx" ]; then
    curl -L -o models/model-small.onnx https://github.com/isl-org/MiDaS/releases/download/v2_1/model-small.onnx
    echo "✅ Small model downloaded successfully!"
else
    echo "✅ Small model already exists."
fi

# Optionally download the large model
read -p "🤔 Download large model (103MB) for higher accuracy? (y/N): " download_large
if [[ $download_large =~ ^[Yy]$ ]]; then
    echo "📥 Downloading MiDaS large model (103MB) for higher accuracy..."
    if [ ! -f "models/model-f6b98070.onnx" ]; then
        curl -L -o models/model-f6b98070.onnx https://github.com/isl-org/MiDaS/releases/download/v2_1/model-f6b98070.onnx
        echo "✅ Large model downloaded successfully!"
    else
        echo "✅ Large model already exists."
    fi
fi

echo ""
echo "🎉 Setup complete! Available models:"
ls -lh models/*.onnx 2>/dev/null || echo "   No ONNX models found"

echo ""
echo "🚀 To run the application:"
echo "   make && ./fletch_vision"
echo ""
echo "🎮 Controls:"
echo "   ESC - Exit"
echo "   E   - Toggle edge detection" 
echo "   F   - Toggle face detection"
echo "   D   - Toggle depth estimation heat map"
