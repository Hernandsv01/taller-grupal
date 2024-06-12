#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <vector>
#include "../../common/library/thread.h"
#include "../../common/library/socket.h"
#include "lobby_protocol.h"
#include "../Game.h"

class GameManager : public Thread{
private:
    LobbyProtocol protocol;
    bool is_running;
    std::vector<std::unique_ptr<Game>> game_pool;

    uint16_t create_game(GameMatch& match_info);

    void join_game();


public:
    explicit GameManager(Socket& socket);

    void run() override;

    void process_command(MessageType& command);

};


#endif //GAME_MANAGER_H