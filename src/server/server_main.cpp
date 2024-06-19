#include <iostream>

#include "Game.h"
#include "clients/client_listener.h"
#include "game/game_manager.h"
#include "game_pool_monitor.h"

int main(int argc, char* argv[]) {
    // Puerto hardcodeado para mas comodidad de ejecución y pruebas
    // if (argc != 2) {
    //     std::cerr << "Bad program call. Expected " << argv[0] << " <port>\n";
    //     return -1;
    // }
    // Client_listener listener(argv[1]);
    const char* puerto = "15500";
    //version con game manager
    //no deberia tener el puerto hardcodeado!
    Socket socket(puerto);
    
    GamePoolMonitor game_pool_monitor;

    Client_listener listener(socket, game_pool_monitor);
    listener.start();

    std::string input;
    do {
        std::cin >> input;
    } while (input != "q");

    listener.kill();
    listener.stop();
    listener.join();
}
