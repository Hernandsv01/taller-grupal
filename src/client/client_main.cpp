#include <iostream>

#include "lobby/lobby.h"
#include "../../src/common/library/socket.h"
#include "client.h"


int main(int argc, char* argv[]) {
    //std::cout << "Hello client!" << std::endl;
    //Lobby lobby;
    //lobby.exec();
    //Socket&& socket = lobby.get_socket(); //ver si puedo agregar esto.
    //harcodear socket
    const char* hostname = "127.0.0.1";
    const char* servname = "8080";
    Window window;
    Socket socket(hostname, servname);
    Client client(std::move(socket), window);
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
