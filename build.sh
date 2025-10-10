#!/bin/bash
echo "Building FileSystem Simulator..."
g++ -std=c++11 -Wall -Wextra -O2 -o filesystem main.cpp FileSystem.cpp
if [ $? -eq 0 ]; then
    echo "Build successful! Run with: ./filesystem"
    chmod +x filesystem
else
    echo "Build failed!"
    exit 1
fi