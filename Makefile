CXX = clang++
CXXFLAGS = -std=c++11 -Wall -Wextra
SRCDIR = src
OBJDIR = obj
TARGET = fletch_vision
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

all: $(TARGET)

test: $(CAMERA_TEST)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(TARGET): $(OBJDIR) src/main.cpp src/DepthEstimator.cpp
	$(CXX) $(CXXFLAGS) $(ALL_INCLUDES) src/main.cpp src/DepthEstimator.cpp $(ALL_LIBS) -o $(TARGET)

$(CAMERA_TEST): camera_test.cpp
	$(CXX) $(CXXFLAGS) $(OPENCV_INCLUDE) camera_test.cpp $(OPENCV_LIBS) -o $(CAMERA_TEST)

clean:
	rm -rf $(OBJDIR) $(TARGET) $(CAMERA_TEST)

run: $(TARGET)
	./$(TARGET)

run-test: $(CAMERA_TEST)
	./$(CAMERA_TEST)

.PHONY: all clean run test run-test
.PHONY: all clean run