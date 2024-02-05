cmake_minimum_required(VERSION 3.10)
project(MyProject VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directory for header files
include_directories(include)

# Add executable
add_executable(brute src/node.cpp src/tree.cpp src/main.cpp)
