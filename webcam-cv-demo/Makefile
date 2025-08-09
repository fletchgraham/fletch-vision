CXX = clang++
CXXFLAGS = -std=c++11 -Wall -Wextra
SRCDIR = src
OBJDIR = obj
TARGET = webcam_cv_demo

# GLFW paths and flags
GLFW_PREFIX = /opt/homebrew/opt/glfw
GLFW_INCLUDE = -I$(GLFW_PREFIX)/include
GLFW_LIBS = -L$(GLFW_PREFIX)/lib -lglfw -framework OpenGL -framework Cocoa -framework IOKit

all: $(TARGET)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(TARGET): $(OBJDIR) src/main_windowed.cpp
	$(CXX) $(CXXFLAGS) $(GLFW_INCLUDE) src/main_windowed.cpp $(GLFW_LIBS) -o $(TARGET)

clean:
	rm -rf $(OBJDIR) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
.PHONY: all clean run