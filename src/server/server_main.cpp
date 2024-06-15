#include <iostream>

#include "Game.h"
#include "clients/client_listener.h"
#include "game/game_manager.h"

int main(int argc, char* argv[]) {
    // Puerto hardcodeado para mas comodidad de ejecución y pruebas
    // if (argc != 2) {
    //     std::cerr << "Bad program call. Expected " << argv[0] << " <port>\n";
    //     return -1;
    // }
    // Client_listener listener(argv[1]);
    char* puerto = "15500";
    /*
    Client_listener listener(puerto);
    listener.start();

    Game game;

    // Ya se que esto es horrible pero es temporal para que haya al menos un
    // cliente al iniciar el juego
    while (Client_Monitor::getAll().empty()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    game.start();

    std::string input;
    do {
        std::cin >> input;
    } while (input != "q");

    // Se para y joinea los threads del listener y game

    listener.kill();

    listener.stop();
    listener.join();

    game.stop();
    game.join();
    */

    //version con game manager
    //no deberia tener el puerto hardcodeado!
    Socket socket(puerto);
    GameManager game_manager(socket);
    game_manager.start();

    //REVISAR MANEJO DE SOCKET EN EL LISTENER
    Client_listener listener(socket);
    listener.start();

   // while (Client_Monitor::getAll().empty()) {
   //    std::this_thread::sleep_for(std::chrono::milliseconds(200));
   // }

    std::string input;
    do {
        std::cin >> input;
    } while (input != "q");

    listener.kill();
    listener.stop();
    listener.join();
    game_manager.kill();
    game_manager.stop();
    game_manager.join();

}
