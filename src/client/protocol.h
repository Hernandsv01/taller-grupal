#ifndef CLIENT_PROTOCOL_H
#define CLIENT_PROTOCOL_H

#include "../common/socket.h"
#include "../common/dtos.h"

class ClientProtocol{
private:
    Socket& socket;

public:
    ClientProtocol(Socket& socket);

    //envia al server la tecla presionada
    void send_action(ActionType& key_event);

    void receive_ticks();

};

#endif //CLIENT_PROTOCOL_H
