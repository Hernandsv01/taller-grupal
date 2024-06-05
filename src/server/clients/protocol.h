#include <netinet/in.h>

#include <iostream>
#include <vector>

#include "../../common/Update.h"
#include "../../common/library/socket.h"
#include "ActionType.h"

#ifndef PROTOCOL_H
#define PROTOCOL_H

class Protocol {
   private:
    Socket& skt;
    bool was_closed;

   public:
    explicit Protocol(Socket& skt) : skt(skt), was_closed(false) {}

    bool sendData(std::vector<Update> payload);
    bool receiveData(ActionType* result);
    bool connectionClosed();
};

#endif  // PROTOCOL_H
