#include "server_client.h"

void Client_sender::run() {
    while (is_running) {
        std::vector<Update::Update_new> result;
        try {
            result = outputQueue.pop();
            protocol.sendData(result);
        } catch (const ClosedQueue& e) {
            is_running = false;
            break;
        } catch (const ClosedConnectionError& e) {
            is_running = false;
            break;
        } catch (const LibError& e) {
            is_running = false;
            break;
        }
    }
}

void Client_sender::addToQueue(std::vector<Update::Update_new> const& result) {
    try {
        outputQueue.try_push(result);
    } catch (const ClosedQueue& e) {
        is_running = false;
    }
}

void Client_receiver::run() {
    while (is_running) {
        ActionType action;

        try {
            protocol.receiveData(&action);
            inputQueue.try_push(action);
        } catch (const ClosedConnectionError& e) {
            // si se cerró la conexion, paro el thread.
            is_running = false;
            break;
        } catch (const ClosedQueue& e) {
            is_running = false;
            break;
        } catch (const LibError& e) {
            is_running = false;
            break;
        }
    }
}

ActionType Client_receiver::get_next_action() {
    ActionType message = ActionType::NULL_ACTION;
    try {
        inputQueue.try_pop(message);
    } catch (const ClosedQueue& e) {
        is_running = false;
        message = ActionType::NULL_ACTION;
    }
    return message;
}
