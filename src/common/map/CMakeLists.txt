
cmake_minimum_required(VERSION 3.5)

project(map VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(SOURCES 
    map.cpp
    map.h)


add_executable(test test.cpp ${SOURCES})

add_library(map SHARED ${SOURCES})

target_link_libraries(test PUBLIC yaml-cpp)
target_link_libraries(map PUBLIC yaml-cpp)
