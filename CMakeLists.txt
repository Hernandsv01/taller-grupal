cmake_minimum_required(VERSION 3.22)
project(taller_grupal)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g")

set(CMAKE_CXX_STANDARD 17)

# Hace que todos los ejecutables queden en el path raiz
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Hace que ejecute el cmakelist del lobby_client
add_subdirectory(src/lobby_client)
# Hace que ejecute el cmakelist del editor de niveles
add_subdirectory(src/editor_niveles)

add_subdirectory(src/common/map)

add_subdirectory(src/client/matchEndedGui)


include_directories(src/client)
include_directories(src/client/gui)
include_directories(src/client/gui/SDL2pp/SDL2pp)
#include_directories(src/client/gui/SDL2pp/tests)
include_directories(src/common)
include_directories(src/common/library)
include_directories(src/server)
include_directories(src/server/clients)
include_directories(src/server/model)
include_directories(src/server/model/constants)

# EJECUTABLES PRYEBAS

# src/client/gui/testfiles/testJazzRunRight.cpp
# src/client/gui/testfiles/testJazzStand.cpp
        # src/client/gui/main.cpp


#set(SOURCES_TEST_SDL
        #src/client/gui/testfiles/testStand.cpp
        #src/client/gui/testfiles/testRun.cpp
        #src/client/gui/testfiles/testIntox.cpp
        #src/client/gui/testfiles/testMap.cpp
        #src/client/gui/estado_juego.h
        #src/client/gui/render.h
        #src/client/gui/render.cpp
        #)

set(SOURCES_CLIENT
        src/client/gui/estado_juego.h
        src/client/gui/gui_loop.cpp
        src/client/gui/gui_loop.h
        src/client/gui/render.cpp
        src/client/gui/render.h
        src/client/gui/entityGame.h
        src/client/gui/entityGame.cpp
        src/client/gui/entityFactory.h
        src/client/gui/entityFactory.cpp
        src/client/gui/textureManager.h
        src/client/gui/textureManager.cpp
        src/client/gui/playableCharacter.h
        src/client/gui/playableCharacter.cpp
        src/client/eventlistener.cpp
        src/client/eventlistener.h
        src/client/mapper.cpp
        src/client/mapper.h
        src/client/protocol.cpp
        src/client/protocol.h
        src/client/update_queue.cpp
        src/client/update_queue.h
        src/client/updater.cpp
        src/client/updater.h
        src/client/client.h
        src/client/client.cpp
        
        src/client/client_main.cpp
        src/server/model/constants/Ammo_data.h
)

set(SOURCES_COMMON
src/common/library/liberror.cpp
        src/common/library/liberror.h
        src/common/library/queue.h
        src/common/library/resolver.cpp
        src/common/library/resolver.h
        src/common/library/resolvererror.cpp
        src/common/library/resolvererror.h
        src/common/library/socket.cpp
        src/common/library/socket.h
        src/common/library/thread.h
        src/common/Update.h
        src/common/Update.cpp
)

set(SOURCES_SERVER
src/server/clients/server_client.cpp
        src/server/clients/server_client.h
        src/server/clients/client_listener.cpp
        src/server/clients/client_listener.h
        src/server/clients/protocol.cpp
        src/server/clients/protocol.h
        src/server/model/constants/game_status.h
        src/server/model/Dynamic_entity.h
        src/server/model/Enemy.h
        src/server/model/Player.h
        src/server/Game.cpp
        src/server/Game.h      
        src/server/server_main.cpp
        src/server/physics/physics.h
        src/server/physics/physics.cpp
        src/server/model/Bullet.h
        src/server/model/Pickup.h
        src/server/model/constants/pickup_type.h
        src/server/game/lobby_manager.h
        src/server/game/lobby_manager.cpp
        src/server/game/lobby_protocol.h
        src/server/game/lobby_protocol.cpp
        src/server/game_pool_monitor.h
        src/server/loader/config.h
        src/server/loader/config.cpp
        src/server/model/constants/Ammo_data.h
)

add_executable(server ${SOURCES_SERVER} ${SOURCES_COMMON})
add_executable(client ${SOURCES_CLIENT} ${SOURCES_COMMON})
#add_executable(testsdl ${SOURCES_TEST_SDL})

find_package(Threads REQUIRED)
target_link_libraries(server PRIVATE Threads::Threads)
target_link_libraries(client PRIVATE Threads::Threads)
target_link_libraries(client PRIVATE GuiLobby)
target_link_libraries(client PRIVATE MatchEndedGui)

#target_link_libraries(testsdl PRIVATE Threads::Threads)
target_link_libraries(server PRIVATE map)
target_link_libraries(client PRIVATE map)


set(SDL2PP_WITH_IMAGE YES)
set(SDL2PP_WITH_TTF YES)
add_subdirectory(src/client/gui/SDL2pp)


target_link_libraries(client PUBLIC SDL2pp)
#target_link_libraries(testsdl PUBLIC SDL2pp)

add_compile_options(-lpthread)
