# Compiler
CXX = clang++

# Compiler flags
CXXFLAGS = -Wall -O2 -std=c++17

# Use pkg-config to get SDL3 flags (Homebrew version)
SDL_CFLAGS  := $(shell pkg-config --cflags sdl3)
SDL_LDFLAGS := $(shell pkg-config --libs sdl3)

# Source files
SOURCES = main.cpp Renderer.cpp resource_manager.cpp event_manager.cpp time.cpp updatable_object.cpp character.cpp sprite_renderer.cpp input_map.cpp input_manager.cpp block.cpp vector3D.cpp vector2D.cpp collider_2d.cpp collider_3d.cpp game_object.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Target executable
TARGET = TestApp

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) $(SDL_LDFLAGS) -o $(TARGET)

# Build object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(SDL_CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET)

# Phony targets
.PHONY: all clean
