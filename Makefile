# Makefile

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -I/opt/homebrew/include 

#SDL flags
SDL_CFLAGS := $(shell sdl2-config --cflags)
SDL_LIBS   := $(shell sdl2-config --libs)

# Sources and objects
SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst src/%.cpp,src/%.o,$(SRC))

# Output binary
TARGET = sim 

# Default rule
all: $(TARGET)

# Linking
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(SDL_LIBS)

# Compile .cpp to .o
src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) $(SDL_CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f src/*.o $(TARGET)
