set(SOURCE
    map.cpp)

set(HEADERS
    map.h)

add_executable(main.out map.cpp)

target_link_libraries(main.out PUBLIC yaml-cpp)