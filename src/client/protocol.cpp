#include "protocol.h"
#include <sstream>
#include <vector>

ClientProtocol::ClientProtocol(Socket& socket) : socket(socket){}

//envio de a un byte ( o sea una accion)
void ClientProtocol::send_action(ActionType& key_event) {
    bool was_closed = false;
    this->socket.sendall(&key_event, sizeof(uint8_t), &was_closed);
    //
}

//recive el paquete del server
std::vector<Update> ClientProtocol::receive_ticks() {
    bool was_closed = false;
    uint8_t quantity = 0x00;
    std::vector<Update> update;
    this->socket.recvall(&quantity, sizeof(uint8_t), &was_closed);
    //
    this->socket.recvall(&update.data(), sizeof(Update)*quantity, &was_closed); //falta el size of what que no me acuerdo como era
    //
    return update;
}

