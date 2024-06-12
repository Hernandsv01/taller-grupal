#include "protocol.h"

#include <netinet/in.h>

#include <iostream>
#include <sstream>

ClientProtocol::ClientProtocol(Socket& socket) : socket(socket) {}

// envio de a un byte ( o sea una accion)
void ClientProtocol::send_action(ActionType& key_event) {
    this->socket.sendall(&key_event, sizeof(ActionType));
    //
}

// recive el paquete del server
std::vector<Update> ClientProtocol::receive_ticks() {
    uint8_t quantity = 0x00;
    this->socket.recvall(&quantity, sizeof(uint8_t));
    //
    std::vector<Update> updates(quantity);
    this->socket.recvall(updates.data(), sizeof(Update) * quantity);
    //
    for (int i = 0; i < updates.size(); ++i) {
        updates[i].id = ntohs(updates[i].id);
        updates[i].value = ntohl(updates[i].value);
    }
    return updates;
}

void ClientProtocol::kill() { socket.close_and_shutdown(); }
