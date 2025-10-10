# Makefile for FileSystem Simulator
# Author: Your Name
# Version: 1.0

# Compiler settings
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
TARGET = filesystem
SOURCES = main.cpp FileSystem.cpp
HEADERS = FileSystem.h
OBJECTS = $(SOURCES:.cpp=.o)

# Default target
all: $(TARGET)

# Build the main executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)
	@echo "Build complete! Run with: ./$(TARGET)"

# Compile source files to object files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET)
	@echo "Clean complete!"

# Install (copy to system path)
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/
	@echo "Installed to /usr/local/bin/"

# Uninstall
uninstall:
	rm -f /usr/local/bin/$(TARGET)
	@echo "Uninstalled from /usr/local/bin/"

# Run the program
run: $(TARGET)
	./$(TARGET)

# Debug build
debug: CXXFLAGS += -g -DDEBUG
debug: $(TARGET)

# Release build (optimized)
release: CXXFLAGS += -O3 -DNDEBUG
release: clean $(TARGET)

# Check for memory leaks (requires valgrind)
memcheck: debug
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)

# Format code (requires clang-format)
format:
	clang-format -i $(SOURCES) $(HEADERS)

# Show help
help:
	@echo "Available targets:"
	@echo "  all      - Build the program (default)"
	@echo "  clean    - Remove build artifacts"
	@echo "  run      - Build and run the program"
	@echo "  debug    - Build with debug symbols"
	@echo "  release  - Build optimized release version"
	@echo "  install  - Install to system path"
	@echo "  uninstall- Remove from system path"
	@echo "  memcheck - Run with valgrind (debug build)"
	@echo "  format   - Format source code"
	@echo "  help     - Show this help message"

# Declare phony targets
.PHONY: all clean install uninstall run debug release memcheck format help