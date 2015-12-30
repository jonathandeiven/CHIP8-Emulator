cmake_minimum_required(VERSION 3.4.1)
project(Chip8-Emulator)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set (SOURCE_FILES src/main.cpp src/cpu.cpp src/window.cpp)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)

include_directories("include")
add_executable (emulator ${SOURCE_FILES})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

find_package(SDL2 REQUIRED)

target_include_directories(emulator SYSTEM PRIVATE "${SDL2_INCLUDE_DIR}")
target_link_libraries(emulator "${SDL2_LIBRARY}")