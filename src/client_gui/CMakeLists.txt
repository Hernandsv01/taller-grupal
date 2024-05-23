cmake_minimum_required(VERSION 2.9)
project(client_gui)

SET(TARGET_H
    gui_loop.h  
   )

SET(TARGET_SRC
    gui_loop.cpp
    main.cpp    
)


add_executable(client_gui ${TARGET_SRC})

SET(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)