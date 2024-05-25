#include "client_eventlistener.h"

EventListener::EventListener(Socket socket) {
    protocol(std::move(socket));
}

void EventListener::listen() {

}
