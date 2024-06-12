#include "lobby.h"

Lobby::Lobby() : lobbyProtocol() {}

void Lobby::checkProtocolOrError(const char* error) {
    std::string str_error = "Primero debe conectarse a un servidor, antes de ";
    str_error += error;

    if (!lobbyProtocol) {
        throw std::logic_error(str_error.c_str());
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

    lobbyProtocol->joinMatch(id);
}

// Devuelve ID de partida creada
uint16_t Lobby::createMatch(const std::string& selected_map,
                            const std::string& matchName) {
    checkProtocolOrError("crear una partida.");

    return lobbyProtocol->createMatch(matchName, selected_map);
}

Lobby::~Lobby() {
    if (lobbyProtocol) {
        delete lobbyProtocol;
    }
}
