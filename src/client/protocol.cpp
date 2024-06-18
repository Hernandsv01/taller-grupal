#include "protocol.h"

#include <netinet/in.h>

#include <iostream>
#include <sstream>

ClientProtocol::ClientProtocol(Socket &socket) : socket(socket) {}

// envio de a un byte ( o sea una accion)
void ClientProtocol::send_action(ActionType &key_event) {
    this->socket.sendall(&key_event, sizeof(ActionType));
    //
}

// recive el paquete del server
std::vector<Update::Update_new> ClientProtocol::receive_ticks() {
    // Recibo la cantidad de bytes que tengo que leer.
    // Esto no se correlaciona de ninguna manera con la cantidad de updates
    uint16_t number_of_bytes;
    this->socket.recvall(&number_of_bytes, sizeof(number_of_bytes));

    number_of_bytes = ntohs(number_of_bytes);

    // Creo un buffer de esa cantidad de bytes, y lo escribo con lo que recibo
    // por socket
    std::vector<uint8_t> buffer(number_of_bytes);
    this->socket.recvall(buffer.data(), number_of_bytes);

    // Deserializo el buffer en un vector de updates
    std::vector<Update::Update_new> updates =
        Update::Update_new::deserialize_all(std::move(buffer));

    return updates;
}

void ClientProtocol::kill() { socket.close_and_shutdown(); }
