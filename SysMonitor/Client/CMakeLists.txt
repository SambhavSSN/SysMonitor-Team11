cmake_minimum_required(VERSION 3.10)
project(Client)

set(CMAKE_CXX_STANDARD 11)


# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/systemInfoCollector.cpp
    src/main.cpp
)

# Executable
add_executable(Client ${SOURCES})


