cmake_minimum_required(VERSION 3.4.1)
project(Chip8-Emulator)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set (SOURCE_FILES src/main.cpp src/cpu.cpp)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)

include_directories("include")
add_executable (emulator src/main.cpp src/cpu.cpp)
