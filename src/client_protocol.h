#ifndef CLIENT_PROTOCOL_H
#define CLIENT_PROTOCOL_H

#include "common_socket.h"

class ClientProtocol{
private:
    Socket socket;

public:
    ClientProtocol();

    void receive_msj();

    void send_msj();

};

#endif //CLIENT_PROTOCOL_H
