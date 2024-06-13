#include "lobby.h"

#include <tuple>

Lobby::Lobby() : lobbyProtocol() {}

void Lobby::checkProtocolOrError(const char* error) const {
    std::string str_error = "Primero debe conectarse a un servidor, antes de ";
    str_error += error;

    if (!lobbyProtocol) {
        throw std::logic_error(str_error.c_str());
    }
}

void Lobby::checkIsConnectedToMatch() const {
    if (!hasConnectedToMatch) {
        throw std::logic_error("No se encuentra conectado a una partida.");
    }
}

void Lobby::connectToServer(const std::string& ip, const std::string& port) {
    if (lobbyProtocol) {
        throw std::logic_error("Ya se encuentra conectado a un servidor");
    }

    lobbyProtocol = new LobbyProtocol(ip.c_str(), port.c_str());
}

void Lobby::desconnectFromServer() {
    checkProtocolOrError("desconectarse de él.");

    delete lobbyProtocol;
    lobbyProtocol = nullptr;
}

std::vector<GameMatch> Lobby::getServerMatches() {
    checkProtocolOrError("solicitar partidas.");

    return lobbyProtocol->getGameMatches();
}

void Lobby::connectToMatch(u_int16_t id) {
    checkProtocolOrError("conectarse a una partida.");

    std::pair<uint16_t, std::string> result = lobbyProtocol->joinMatch(id);

    hasConnectedToMatch = true;
    assigned_player_id = result.first;
    assigned_map_name = result.second;
}

// Devuelve ID de partida creada
match_id Lobby::createMatch(const std::string& selected_map,
                            const std::string& matchName) {
    checkProtocolOrError("crear una partida.");

    return lobbyProtocol->createMatch(matchName, selected_map);
}
bool Lobby::isConnectedToMatch() const { return hasConnectedToMatch; }

Socket Lobby::getMatchConnection() {
    checkProtocolOrError("obtener la conexión a la partida.");
    checkIsConnectedToMatch();

    Socket socket_a_devolver = lobbyProtocol->getSocket();

    // Indica que ya no esta conectado a un match, y elimina el protocolo que ya
    // no contiene el socket, por lo que no es funcional.
    hasConnectedToMatch = false;
    delete lobbyProtocol;
    lobbyProtocol = nullptr;

    return socket_a_devolver;
}

std::pair<uint16_t, std::string> Lobby::getPlayerIdAndMapName() {
    checkProtocolOrError("obtener la conexión a la partida.");
    checkIsConnectedToMatch();

    std::pair<uint16_t, std::string> playerIdAndMapName =
        std::make_pair(assigned_player_id, assigned_map_name);

    return playerIdAndMapName;
}

Lobby::~Lobby() {
    if (lobbyProtocol) {
        delete lobbyProtocol;
    }
}
