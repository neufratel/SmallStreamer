cmake_minimum_required(VERSION 2.8)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

project(SPlay)

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fpermissive -lao -lmpg123")
# set(CMAKE_INCLUDE_CURRENT_DIR ON)

# build subdirs

#find_package(mpg123)
#find_package(ao)

add_executable(${PROJECT_NAME} main.cpp streamcreator.h sender.cpp) 
target_link_libraries(${PROJECT_NAME} ao mpg123)
find_package(Qt4Core)
find_package(Qt4Gui)
find_package(Qt4Network)
find_package(Qt4Widgets)
target_link_libraries(${PROJECT_NAME} pthread Qt4::Gui Qt4::Widgets Qt4::Core Qt4::Network)
add_subdirectory(server)

