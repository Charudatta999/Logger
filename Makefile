# Compiler and flags
CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -pthread # Added -pthread for thread support
DEBUGFLAGS := -g # Debug flag
OPTFLAGS := -O2 # Optimization flag
OPTIMIZATION ?= $(OPTFLAGS) # Default to OPTFLAGS if not overridden

# Directories
INC_DIR := inc
SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin

# Source files
SRC := $(SRC_DIR)/FileHandler.cpp $(SRC_DIR)/Logger.cpp main.cpp

# Include files
INC := $(INC_DIR)/Logger.hpp $(INC_DIR)/FileHandler.hpp

# Object files
OBJ := $(addprefix $(BUILD_DIR)/, $(notdir $(SRC:.cpp=.o)))
# Executable name
TARGET := $(BIN_DIR)/main

# Default target
all: directories $(TARGET)

# Create directories
directories:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BIN_DIR)

# Compile source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(INC)
	$(CXX) $(CXXFLAGS) $(OPTIMIZATION) -I$(INC_DIR) -c $< -o $@

# Handle main.cpp, which is outside the src directory
$(BUILD_DIR)/main.o: main.cpp $(INC)
	$(CXX) $(CXXFLAGS) $(OPTIMIZATION) -I$(INC_DIR) -c $< -o $@

# Link object files to create the executable
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OPTIMIZATION) -I$(INC_DIR) $^ -o $@

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Phony targets
.PHONY: all clean directories debug release

# Debug target:  make debug
debug:
	$(MAKE) OPTIMIZATION=$(DEBUGFLAGS) all # Override OPTIMIZATION for debug build

# Release target: make release
release:
	$(MAKE) OPTIMIZATION=$(OPTFLAGS) all
