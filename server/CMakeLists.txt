cmake_minimum_required(VERSION 2.8)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

project(server)

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fpermissive -lao -lmpg123 -lpthread -lboost_system -lboost_date_time -lboost_thread ")
# set(CMAKE_INCLUDE_CURRENT_DIR ON)

# build subdirs

#find_package(mpg123)
#find_package(ao)
include_directories(${YOUR_DIRECTORY} )
include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/utils)
add_executable(${PROJECT_NAME} main.cpp server.cpp) 
target_link_libraries(${PROJECT_NAME} ao mpg123 pthread boost_system utils )

