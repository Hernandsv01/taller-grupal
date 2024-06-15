#include "protocol.h"

#include <netinet/in.h>

/**
 * Send data to a certain ip and port
 * @param payload
 * @return void
 */
void Protocol::sendData(std::vector<Update::Update_new> payload) {
    std::vector<uint8_t> buffer;

    for (Update::Update_new update : payload) {
        std::vector<uint8_t> parcial_buffer = update.serialize();
        buffer.insert(buffer.end(), parcial_buffer.begin(),
                      parcial_buffer.end());
    }

    uint16_t size = buffer.size();
    size = htons(size);
    skt.sendall(&size, sizeof(size));

    skt.sendall(buffer.data(), buffer.size());
}

/**
 *
 * @param result
 * @return void
 */
void Protocol::receiveData(ActionType* result) {
    skt.recvall(result, sizeof(ActionType));
}
