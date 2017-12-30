project(yt_loader)

cmake_minimum_required(VERSION 2.8)

# Sets up the sources
set(LOADER_SOURCES src/main.cpp)

# Sets the compiler flags
set(CMAKE_CXX_FLAGS "-std=c++11 -g -Wall")

find_package(Boost COMPONENTS system filesystem REQUIRED)

# Includes the directory for the header files
include_directories(${CMAKE_SOURCE_DIR}/include)

# Builds the executable
add_executable(main ${LOADER_SOURCES})

# Dynamically links curl to the executable (main)
target_link_libraries(main curl ${Boost_LIBRARIES})
