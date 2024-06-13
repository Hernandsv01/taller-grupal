#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <vector>
#include "../../common/library/thread.h"
#include "../../common/library/socket.h"
#include "lobby_protocol.h"
#include "../Game.h"
#include "../../common/map/map.h"

class GameManager : public Thread{
private:
    LobbyProtocol protocol;
    bool is_running;
    std::vector<std::unique_ptr<Game>> game_pool;
    std::vector<Map> map_list;

    uint8_t create_game(GameMatch& match_info);

    uint16_t join_game(uint8_t game_id);


public:
    explicit GameManager(Socket& socket);

    void run() override;

    void process_command(MessageType& command);

   void kill();

};


#endif //GAME_MANAGER_H