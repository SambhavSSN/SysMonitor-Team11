cmake_minimum_required(VERSION 3.10)
project(Client)

set(CMAKE_CXX_STANDARD 11)


# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/main.cpp
    src/DatabaseManager.cpp
)

# Executable
add_executable(Server ${SOURCES})

# Link against the MySQL connector library
find_library(MYSQLCPPCONN_LIB mysqlcppconn)
target_link_libraries(Server PRIVATE ${MYSQLCPPCONN_LIB})
