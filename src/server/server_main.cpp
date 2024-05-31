#include <iostream>
#include "clients/client_listener.h"
#include "Game.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Bad program call. Expected " << argv[0] << " <port>\n";
        return -1;
    }
    Client_listener listener(argv[1]);
    listener.start();

    Game game;

    // Ya se que esto es horrible pero es temporal para que haya al menos un cliente al iniciar el juego
    while (Client_Monitor::getAll().empty()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    game.start();

    std::string input;
    do {
        std::cin >> input;
    } while (input != "q");
}