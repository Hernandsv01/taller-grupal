#include "client.h"

void Client_sender::run() {
    while (is_running) {
        std::vector<Update> result;
        try {
            result = outputQueue.pop();
        } catch (const ClosedQueue& e) {
            is_running = false;
            break;
        }
        protocol.sendData(result);
    }
}

void Client_sender::addToQueue(std::vector<Update> const& result) { outputQueue.try_push(result); }

void Client_receiver::run() {
    while (is_running) {
        ActionMessage action;
        bool successful = protocol.receiveData(&action);
        if (!successful) {
            is_running = false;
            break;
        }
        inputQueue.try_push(action);
    }
}

ActionType Client_receiver::get_next_action() {
    return ActionType::NULL_ACTION;
}
