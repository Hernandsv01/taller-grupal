set(CMAKE_CXX_STANDARD 17)


set(SOURCES 
    map.cpp
    test.cpp
    map.h)


add_executable(test ${SOURCES})

target_link_libraries(test PUBLIC yaml-cpp)
