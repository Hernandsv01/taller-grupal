#include "protocol.h"
#include <sstream>

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
    std::vector<Update> update(quantity);
    this->socket.recvall(update.data(), sizeof(Update)*quantity, &was_closed); //falta el size of what que no me acuerdo como era
    //
    return update;
}

void ClientProtocol::kill() {
    socket.shutdown(2);
    socket.close();
}

