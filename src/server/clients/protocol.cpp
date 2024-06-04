#include "protocol.h"

/**
 * Send data to a certain ip and port
 * @param payload
 * @return true if operation was successful, false otherwise
 */
bool Protocol::sendData(std::vector<Update> payload) {
    if (was_closed) return false;

    uint8_t size = payload.size();
    skt.sendall(&size, sizeof(size), &was_closed);

    if (was_closed) return false;

    skt.sendall(&payload, sizeof(Update) * payload.size(), &was_closed);

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

    skt.recvall(result, sizeof(ActionMessage), &was_closed);

    if (was_closed) return false;
    return true;
}

bool Protocol::connectionClosed() { return was_closed; }
