project(yt_loader)

cmake_minimum_required(VERSION 2.8)

# Sets up the sources
set(LOADER_SOURCES src/Main.cpp)

# Sets the compiler flags
set(CMAKE_CXX_FLAGS "-std=c++11 -g -Wall")

find_package(Boost COMPONENTS system filesystem REQUIRED)

# Includes the directory for the header files
include_directories(${CMAKE_SOURCE_DIR}/include)

# Builds the executable
add_executable(Main ${LOADER_SOURCES})

# Dynamically links curl to the executable (Main)
target_link_libraries(Main curl ${Boost_LIBRARIES})
