#include <iostream>

// #include "lobby/lobby.h"
#include "../../src/common/library/socket.h"
#include "../lobby_client/gui_lobby.h"
#include "client.h"

/*
 * Se harcodea un hostname y un servername para generar una conexion.
 * Hay codigo comentado relacionado al lobby que todavia no esta integrado.
 * Y uno de prueba que solo usaba el render.
 */
int main(int argc, char* argv[]) {
    // std::cout << "Hello client!" << std::endl;
    // Lobby lobby;
    // lobby.exec();
    // Socket&& socket = lobby.get_socket(); //ver si puedo agregar esto.
    // harcodear socket

    GuiLobby gui_lobby(argc, argv);
    gui_lobby.ejecutar(argc, argv);

    const char* hostname = "127.0.0.1";
    const char* servname = "15500";
    SDL2pp::SDL sdl(SDL_INIT_VIDEO);
    SDL2pp::Window window("TEST", SDL_WINDOWPOS_CENTERED,
                          SDL_WINDOWPOS_CENTERED, 800, 600,
                          SDL_WindowFlags::SDL_WINDOW_RESIZABLE);

    Socket socket(hostname, servname);
    Client client(std::move(socket), window);

    // Como esta funcion bloquea hasta que el cliente se cierre
    // (o termine con un error), no es necesario esperar
    // a que el usuario envíe 'q' para cerrar el cliente.
    client.exec();

    return 0;
    // Estructura del main
    /*
     * create_lobby
     * lobby_get_socket
     * create_client
     *      - protocolo inicializar partida
     *      - eventlistener.start
     *      - gui_loop
     *          - renderer
     *          - updater
     */
}
