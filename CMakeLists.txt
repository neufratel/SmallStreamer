cmake_minimum_required(VERSION 2.8)
project(SmallStreamer)

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# build subdirs
add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(utils)
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/utils)
