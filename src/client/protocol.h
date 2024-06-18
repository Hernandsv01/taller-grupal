#ifndef CLIENT_PROTOCOL_H
#define CLIENT_PROTOCOL_H

#include "../common/library/socket.h"
#include "ActionType.h"
#include "Update.h"
#include <vector>

class ClientProtocol{
   private:
    Socket& socket;

   public:
    explicit ClientProtocol(Socket& socket);

    //envia al server la tecla presionada
    void send_action(ActionType& key_event);

    std::vector<Update::Update_new> receive_ticks();

    void kill();
};

#endif //CLIENT_PROTOCOL_H
