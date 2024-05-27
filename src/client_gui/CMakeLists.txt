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

add_executable(client_gui ${TARGET_SRC})

# add compilation flags
SET(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
add_definitions(-DDATA_PATH="${PROJECT_SOURCE_DIR}/data")
include_directories(${SDL2PP_INCLUDE_DIRS})

# define targets
add_executable(testJazzStand testfiles/testJazzStand.cpp render.cpp)
target_link_libraries(testJazzStand PUBLIC SDL2pp)

add_executable(testJazzRunRight testfiles/testJazzRunRight.cpp render.cpp)
target_link_libraries(testJazzRunRight PUBLIC SDL2pp)
