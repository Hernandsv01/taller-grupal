cmake_minimum_required(VERSION 2.9)
project(client_gui)

#set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g")

SET(TARGET_H
        gui_loop.h
   )

SET(TARGET_SRC
        gui_loop.cpp
        main.cpp
)

#SET(TEST_TARGET)

# set up SDL2pp library
set(SDL2PP_WITH_IMAGE YES)
set(SDL2PP_WITH_MIXER YES)
set(SDL2PP_WITH_TTF YES)
add_subdirectory(SDL2pp)

#add_executable(client_gui ${TARGET_SRC})

# add compilation flags
SET(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
add_definitions(-DDATA_PATH="${CMAKE_CURRENT_SOURCE_DIR}/data")
include_directories(${SDL2PP_INCLUDE_DIRS})

# define targets
add_executable(testMusic testfiles/testMusic.cpp soundManager.cpp)
target_link_libraries(testMusic PUBLIC SDL2pp)