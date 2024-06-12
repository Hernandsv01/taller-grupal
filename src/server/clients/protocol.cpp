#include "protocol.h"

#include <netinet/in.h>

/**
 * Send data to a certain ip and port
 * @param payload
 * @return void
 */
void Protocol::sendData(std::vector<Update> payload) {
    for (int i = 0; i < payload.size(); ++i) {
        payload[i].id = htons(payload[i].id);
        payload[i].value = htonl(payload[i].value);
    }

    uint8_t size = payload.size();
    skt.sendall(&size, sizeof(size));

    for (int i = 0; i < payload.size(); ++i) {
        skt.sendall(&payload[i], sizeof(payload[i]));
    }
}

/**
 *
 * @param result
 * @return void
 */
void Protocol::receiveData(ActionType* result) {
    skt.recvall(result, sizeof(ActionType));
}
