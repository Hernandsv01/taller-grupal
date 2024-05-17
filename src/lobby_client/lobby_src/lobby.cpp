#include "lobby.h"

Lobby::Lobby(): protocolo() {
}

ProtocoloLobby&Lobby::obtener_protocolo_o_error(char *error) {
    std::string str_error = "Primero debe conectarse a un servidor, antes de ";
    str_error += error;

    if (!protocolo.has_value()) {
        throw std::logic_error(str_error.c_str());
    }

    return protocolo.value();
}


void Lobby::conectar_a_servidor(const std::string& ip, const std::string& puerto) {
    if (protocolo.has_value()) {
        throw std::logic_error("Ya se encuentra conectado a un servidor");
    }

    protocolo.emplace(ProtocoloLobby(ip.c_str(), puerto.c_str()));
}

std::vector<Partida> Lobby::obtener_partidas_servidor() {
    ProtocoloLobby& protocolo_valido = obtener_protocolo_o_error("solicitar partidas.");

    return protocolo_valido.obtener_partidas();
}

void Lobby::conectarse_a_partida(u_int16_t id) {
    ProtocoloLobby& protocolo_valido = obtener_protocolo_o_error("conectarse a una partida.");

    protocolo_valido.conectar_a_partida(id);
}


//TODO: agregar mapa. nombre? algo más?
//Devuelve ID de partida creada
uint16_t Lobby::crear_partida(std::string mapa_selecionado, uint8_t cant_jugadores, std::string nombre_partida) {
    ProtocoloLobby& protocolo_valido = obtener_protocolo_o_error("crear una partida.");

    Partida partida = {0, cant_jugadores, 0, nombre_partida};

    return protocolo_valido.crear_partida(partida);
}

