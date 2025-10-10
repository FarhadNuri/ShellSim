@echo off
echo Building FileSystem Simulator...
g++ -std=c++11 -Wall -Wextra -O2 -o filesystem.exe main.cpp FileSystem.cpp
if %errorlevel% == 0 (
    echo Build successful! Run with: filesystem.exe
) else (
    echo Build failed!
    pause
)