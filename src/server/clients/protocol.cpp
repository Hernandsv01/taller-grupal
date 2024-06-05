#include "protocol.h"
#include <netinet/in.h>

/**
 * Send data to a certain ip and port
 * @param payload
 * @return true if operation was successful, false otherwise
 */
bool Protocol::sendData(std::vector<Update> payload) {
    if (was_closed) return false;
    for (int i = 0; i < payload.size(); ++i) {
        payload[i].id = htons(payload[i].id);
        payload[i].value = htonl(payload[i].value);
    }

    uint8_t size = payload.size();
    skt.sendall(&size, sizeof(size), &was_closed);

    if (was_closed) return false;

    for (int i = 0; i < payload.size(); ++i) {
        skt.sendall(&payload[i], sizeof(payload[i]), &was_closed);
    }

    if (was_closed) return false;
    return true;
}

/**
 *
 * @param result
 * @return true if operation was successful, false otherwise
 */
bool Protocol::receiveData(ActionType* result) {
    if (was_closed) return false;

    skt.recvall(result, sizeof(ActionType), &was_closed);

    if (was_closed) return false;
    return true;
}

bool Protocol::connectionClosed() { return was_closed; }
