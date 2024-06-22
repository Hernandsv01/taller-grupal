
cmake_minimum_required(VERSION 3.5)

project(map VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(SOURCES 
    map.cpp
    map.h)

# Se ejecuta solamente si se esta ejecutando unicamente este cmake
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    add_executable(test test.cpp ${SOURCES})
    target_link_libraries(test PUBLIC yaml-cpp)
endif()

add_library(map SHARED ${SOURCES})
target_link_libraries(map PUBLIC yaml-cpp)

# Agrega una ubicacion fija para los mapas.
target_compile_definitions(map PUBLIC MAP_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../../maps/")
