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

   public:
    explicit Protocol(Socket& skt) : skt(skt) {}

    void sendData(std::vector<Update> payload);
    void receiveData(ActionType* result);
};

#endif  // PROTOCOL_H
