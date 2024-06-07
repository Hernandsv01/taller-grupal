#include <iostream>

// #include "lobby/lobby.h"
#include "../../src/common/library/socket.h"
#include "client.h"

int main(int argc, char* argv[]) {
    // std::cout << "Hello client!" << std::endl;
    // Lobby lobby;
    // lobby.exec();
    // Socket&& socket = lobby.get_socket(); //ver si puedo agregar esto.
    // harcodear socket
    const char* hostname = "127.0.0.1";
    const char* servname = "15500";
    SDL2pp::SDL sdl(SDL_INIT_VIDEO);
    SDL2pp::Window window("TEST", SDL_WINDOWPOS_CENTERED,
                          SDL_WINDOWPOS_CENTERED, 800, 600,
                          SDL_WindowFlags::SDL_WINDOW_RESIZABLE);

    // INICIO Client main normal
    Socket socket(hostname, servname);
    Client client(std::move(socket), window);

    client.exec();

    std::string input;
    do {
        std::cin >> input;
    } while (input != "q");

    return 0;
    // FINAL  Client main normal

    // INICIO CLIENT MAIN ONLY RENDER
    // ¿ESTE LO USO PARA PROBAR PORQUE NO RENDERIZA NADA?
    // Render render(window);

    // PlayerState personajeJazz;
    // personajeJazz.position = Position{100, 200};
    // personajeJazz.direction = Direction::Right;
    // personajeJazz.characterType = Jazz;
    // personajeJazz.healthPoints = 10;
    // personajeJazz.score = 0;

    // while (1) {
    //     render.presentPlayer(personajeJazz);
    // }
    // FINAL CLIENT MAIN ONLY RENDER

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
