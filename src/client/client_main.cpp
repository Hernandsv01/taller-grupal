#include <iostream>
#include <utility>

#include "../../src/common/library/socket.h"
#include "../lobby_client/gui_lobby.h"
#include "client.h"
#include "matchEndedGui/matchended.h"

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
        gui_lobby.execute();

        if (!gui_lobby.isConnectedToMatch()) {
            return 0;
        }

        playerIdAndMap = gui_lobby.getPlayerIdAndMapName();
        socket_a_usar = gui_lobby.extractMatchConnection();

    } else {
        const char* hostname = "127.0.0.1";
        const char* servname = "15500";

        Lobby lobby;
        lobby.connectToServer(hostname, servname);

        auto avaibleMatches = lobby.getServerMatches();

        uint8_t match_to_connect;

        if (avaibleMatches.empty()) {
            match_to_connect = lobby.createMatch("map3", "Partida1");
        } else {
            match_to_connect = avaibleMatches[0].id;
        }
        lobby.connectToMatch(match_to_connect);

        playerIdAndMap = lobby.getPlayerIdAndMapName();
        socket_a_usar = lobby.extractMatchConnection();
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

    if (client.matchEnded()) {
        std::vector<PlayerScore> scores;

        scores.push_back({"Bunny1", 1, 100, true});

        MatchEnded matchEnded(argc, argv);
        matchEnded.showWithScores(scores);
    }

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
