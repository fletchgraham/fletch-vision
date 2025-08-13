#!/bin/bash

echo "=== Fletch Vision Demo Launcher ==="
echo ""
echo "Available demos:"
echo "1. Computer Vision Demo (webcam + CV effects)"
echo "2. Simple 3D Cube Demo (interactive 3D graphics)"
echo "3. Build all demos"
echo "4. Exit"
echo ""

while true; do
    read -p "Select demo to run [1-4]: " choice
    case $choice in
        1)
            echo "Launching Computer Vision Demo..."
            echo "Controls: ESC=quit, E=edge detection, F=face detection, D=depth estimation"
            ./fletch_vision
            ;;
        2)
            echo "Launching 3D Cube Demo..."
            echo "Controls: Click and drag to orbit, ESC=quit"
            ./simple_cube_viewer
            ;;
        3)
            echo "Building all demos..."
            make clean && make
            echo "Build complete! Run this script again to launch demos."
            ;;
        4)
            echo "Goodbye!"
            exit 0
            ;;
        *)
            echo "Invalid choice. Please select 1-4."
            ;;
    esac
    echo ""
done
