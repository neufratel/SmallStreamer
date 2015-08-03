cmake_minimum_required(VERSION 2.8)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

project(client)

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fpermissive -lao -lmpg123 -lpthread -lboost_system -lboost_date_time -lboost_thread ")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Network)
find_package(Qt5Widgets)

# build subdirs
include_directories(${YOUR_DIRECTORY} PRIVATE ${CMAKE_SOURCE_DIR}/utils ${CMAKE_SOURCE_DIR}/client/gui)
add_executable(${PROJECT_NAME} main.cpp client.cpp clientcontroler.cpp gui/clientcontrolpanel.cpp controler.cpp playlist.cpp gui/playlistwidget.cpp gui/maincontrolpanel.cpp gui/mainwindow.cpp) 
target_link_libraries(${PROJECT_NAME} ao mpg123 pthread boost_system utils Qt5::Core Qt5::Network)

qt5_use_modules(${PROJECT_NAME} Widgets)



