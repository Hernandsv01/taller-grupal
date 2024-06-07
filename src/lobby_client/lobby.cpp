#include "lobby.h"

Lobby::Lobby() : lobbyProtocol() {}

LobbyProtocol& Lobby::getProtocolOrError(const char* error) {
    std::string str_error = "Primero debe conectarse a un servidor, antes de ";
    str_error += error;

    if (!lobbyProtocol.has_value()) {
        throw std::logic_error(str_error.c_str());
    }

    return lobbyProtocol.value();
}

void Lobby::connectToServer(const std::string& ip, const std::string& port) {
    if (lobbyProtocol.has_value()) {
        throw std::logic_error("Ya se encuentra conectado a un servidor");
    }

    lobbyProtocol.emplace(LobbyProtocol(ip.c_str(), port.c_str()));
}

std::vector<GameMatch> Lobby::getServerMatches() {
    LobbyProtocol& validProtocol = getProtocolOrError("solicitar partidas.");

    return validProtocol.getGameMatches();
}

void Lobby::connectToMatch(u_int16_t id) {
    LobbyProtocol& validProtocol =
        getProtocolOrError("conectarse a una partida.");

    validProtocol.connectToMatch(id);
}

// Devuelve ID de partida creada
uint16_t Lobby::createMatch(const std::string& selected_map,
                            uint8_t playerCount, std::string matchName) {
    LobbyProtocol& validProtocol = getProtocolOrError("crear una partida.");

    GameMatch match = {0, playerCount, 0, matchName};

    return validProtocol.createMatch(match);
}
