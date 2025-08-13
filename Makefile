CXX = clang++
CXXFLAGS = -std=c++11 -Wall -Wextra
SRCDIR = src
OBJDIR = obj
TARGET = fletch_vision
CUBE_DEMO = simple_cube_viewer
CAMERA_TEST = camera_test

# GLFW paths and flags
GLFW_PREFIX = /opt/homebrew/opt/glfw
GLFW_INCLUDE = -I$(GLFW_PREFIX)/include
GLFW_LIBS = -L$(GLFW_PREFIX)/lib -lglfw -framework OpenGL -framework Cocoa -framework IOKit

# OpenCV paths and flags
OPENCV_PREFIX = /opt/homebrew/opt/opencv
OPENCV_INCLUDE = -I$(OPENCV_PREFIX)/include/opencv4
OPENCV_LIBS = -L$(OPENCV_PREFIX)/lib -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs -lopencv_videoio -lopencv_objdetect -lopencv_dnn -lopencv_dnn

# Combined flags
ALL_INCLUDES = $(GLFW_INCLUDE) $(OPENCV_INCLUDE)
ALL_LIBS = $(GLFW_LIBS) $(OPENCV_LIBS)

all: $(TARGET) $(CUBE_DEMO)

test: $(CAMERA_TEST)

cube: $(CUBE_DEMO)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(TARGET): $(OBJDIR) src/main.cpp src/DepthEstimator.cpp src/DepthEstimatorFactory.cpp
	$(CXX) $(CXXFLAGS) $(ALL_INCLUDES) src/main.cpp src/DepthEstimator.cpp src/DepthEstimatorFactory.cpp $(ALL_LIBS) -o $(TARGET)

$(CUBE_DEMO): $(OBJDIR) src/cube_main.cpp src/SimpleCubeViewer.cpp
	$(CXX) $(CXXFLAGS) $(GLFW_INCLUDE) src/cube_main.cpp src/SimpleCubeViewer.cpp $(GLFW_LIBS) -o $(CUBE_DEMO)

$(CAMERA_TEST): camera_test.cpp
	$(CXX) $(CXXFLAGS) $(OPENCV_INCLUDE) camera_test.cpp $(OPENCV_LIBS) -o $(CAMERA_TEST)

clean:
	rm -rf $(OBJDIR) $(TARGET) $(CUBE_DEMO) $(CAMERA_TEST)

run: $(TARGET)
	./$(TARGET)

run-cube: $(CUBE_DEMO)
	./$(CUBE_DEMO)

run-test: $(CAMERA_TEST)
	./$(CAMERA_TEST)

.PHONY: all clean run cube run-cube test run-test