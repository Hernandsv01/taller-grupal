cmake_minimum_required(VERSION 3.22)

project(test_update)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

set(SOURCES_UPDATE
    Update.h
    Update.cpp
    test_update.cpp)

add_executable(update_test ${SOURCES_UPDATE})