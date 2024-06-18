cmake_minimum_required(VERSION 3.5)

project(matchEndedGui VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(PROJECT_SOURCES
        matchendedgui.cpp
        matchendedgui.h
        matchendedgui.ui
)

add_library(matchEndedGui STATIC ${PROJECT_SOURCES})
target_link_libraries(matchEndedGui PUBLIC Qt${QT_VERSION_MAJOR}::Widgets)


if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    add_executable(testGui main.cpp)
    target_link_libraries(testGui PRIVATE matchEndedGui)
endif()