#include "protocol.h"
#include <sstream>
#include <iostream>
#include <netinet/in.h>

ClientProtocol::ClientProtocol(Socket& socket) : socket(socket){}

//envio de a un byte ( o sea una accion)
void ClientProtocol::send_action(ActionType& key_event) {
    bool was_closed = false;
    this->socket.sendall(&key_event, sizeof(ActionType), &was_closed);
    //
}

//recive el paquete del server
std::vector<Update> ClientProtocol::receive_ticks() {
    bool was_closed = false;
    uint8_t quantity = 0x00;
    this->socket.recvall(&quantity, sizeof(uint8_t), &was_closed);
    //
    std::vector<Update> updates(quantity);
    this->socket.recvall(updates.data(), sizeof(Update)*quantity, &was_closed);
    //
    for (int i = 0; i < updates.size(); ++i) {
        updates[i].id = ntohs(updates[i].id);
        updates[i].value = ntohl(updates[i].value);
    }
    return updates;
}

void ClientProtocol::kill() {
    socket.shutdown(2);
    socket.close();
}

