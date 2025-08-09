CXX = clang++
CXXFLAGS = -std=c++11 -Wall -Wextra
SRCDIR = src
OBJDIR = obj
TARGET = webcam_cv_demo

all: $(TARGET)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(TARGET): $(OBJDIR) src/main.cpp
	$(CXX) $(CXXFLAGS) src/main.cpp -o $(TARGET)

clean:
	rm -rf $(OBJDIR) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run