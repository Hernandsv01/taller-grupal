#include "protocol.h"

ClientProtocol::ClientProtocol(Socket& socket) : socket(socket){}

//envio de a un byte ( o sea una accion)
void ClientProtocol::send_action(ActionType& key_event) {
    bool was_closed = false;
    this->socket.sendall(&key_event, sizeof(uint8_t), &was_closed);
    //evaluo if was closed y suelto excep.
}

//recive el paquete del server
void ClientProtocol::receive_ticks() {

}

