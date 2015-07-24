project(utils)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find Qt libraries
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fpermissive -lao -lmpg123 -lpthread -lboost_system -lboost_date_time -lboost_thread ")
add_library(${PROJECT_NAME} SHARED empty.cpp logger.h str_queue.h player.cpp player.h stream.h runnable.h)
target_link_libraries(${PROJECT_NAME} ao mpg123 pthread boost_system)
