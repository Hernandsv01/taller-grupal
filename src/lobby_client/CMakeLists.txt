cmake_minimum_required(VERSION 3.5)

project(lobby_client VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(PROJECT_SOURCES
        lobby.cpp
        lobby.h
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        protocololobby.cpp
        protocololobby.h

        gui_lobby.h

        
        ../common/library/liberror.cpp
        ../common/library/liberror.h
        ../common/library/resolver.cpp
        ../common/library/resolver.h
        ../common/library/resolvererror.cpp
        ../common/library/resolvererror.h
        ../common/library/closedconectionerror.h
        ../common/library/socket.cpp
        ../common/library/socket.h
)

set(EJECUTABLE_SOURCE
    main.cpp)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(lobby_client
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET lobby_client APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(lobby_client SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(lobby_client
            ${PROJECT_SOURCES} ${EJECUTABLE_SOURCE}
        )
    endif()
endif()

add_library(GuiLobby STATIC ${PROJECT_SOURCES})

target_compile_definitions(GuiLobby PUBLIC MAP_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../maps/")


target_link_libraries(GuiLobby PUBLIC Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(lobby_client PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(lobby_client PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(lobby_client)
endif()


add_executable(moc_server moc_server.cpp ../common/library/liberror.cpp
../common/library/liberror.h
../common/library/resolver.cpp
../common/library/resolver.h
../common/library/resolvererror.cpp
../common/library/resolvererror.h
../common/library/closedconectionerror.h
../common/library/socket.cpp
../common/library/socket.h)
