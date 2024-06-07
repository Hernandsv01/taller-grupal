cmake_minimum_required(VERSION 2.9)
project(client_gui)

SET(TARGET_H
        gui_loop.h
   )

SET(TARGET_SRC
        gui_loop.cpp
        main.cpp
)

# set up SDL2pp library
set(SDL2PP_WITH_IMAGE YES)
set(SDL2PP_WITH_TTF YES)
add_subdirectory(SDL2pp)

#add_executable(client_gui ${TARGET_SRC})

# add compilation flags
SET(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
add_definitions(-DDATA_PATH="${CMAKE_CURRENT_SOURCE_DIR}/data")
include_directories(${SDL2PP_INCLUDE_DIRS})

# define targets
add_executable(testStand testfiles/testStand.cpp render.cpp)
target_link_libraries(testStand PUBLIC SDL2pp)

add_executable(testRun testfiles/testRun.cpp render.cpp)
target_link_libraries(testRun PUBLIC SDL2pp)

add_executable(testIntox testfiles/testIntox.cpp render.cpp)
target_link_libraries(testIntox PUBLIC SDL2pp)

add_executable(testMap testfiles/testMap.cpp render.cpp)
target_link_libraries(testMap PUBLIC SDL2pp)


