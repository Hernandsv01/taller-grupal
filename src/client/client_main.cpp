#include <iostream>
#include <utility>

#include "../../src/common/library/socket.h"
#include "../lobby_client/gui_lobby.h"
#include "client.h"

/*
 * Se harcodea un hostname y un servername para generar una conexion.
 * Hay codigo comentado relacionado al lobby que todavia no esta integrado.
 * Y uno de prueba que solo usaba el render.
 */

int main(int argc, char* argv[]) {
    // TODO: REMOVER PARA ENTREGA FINAL

    std::optional<Socket> socket_a_usar;
    auto playerIdAndMap = std::make_pair(0, std::string(""));

    if (argc == 2 && std::string(argv[1]) == "manual") {
        GuiLobby gui_lobby(argc, argv);
        // MAGIA NEGRA: revisar implementacion guilooby
        gui_lobby.execute(argc, argv);

        if (!gui_lobby.isConnectedToMatch()) {
            return 0;
        }

        playerIdAndMap = gui_lobby.getPlayerIdAndMapName();
        socket_a_usar = gui_lobby.getMatchConnection();

    } else {
        const char* hostname = "127.0.0.1";
        const char* servname = "15500";

        Lobby lobby;
        lobby.connectToServer(hostname, servname);
        lobby.connectToMatch(1);

        playerIdAndMap = lobby.getPlayerIdAndMapName();
        socket_a_usar = lobby.getMatchConnection();
    }

    SDL2pp::SDL sdl(SDL_INIT_VIDEO);
    SDL2pp::Window window("TEST", SDL_WINDOWPOS_CENTERED,
                          SDL_WINDOWPOS_CENTERED, 800, 600,
                          SDL_WindowFlags::SDL_WINDOW_RESIZABLE);

    uint16_t player_id = playerIdAndMap.first;
    std::string map_name = playerIdAndMap.second;

    Client client(std::move(socket_a_usar.value()), window, player_id,
                  map_name);

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
