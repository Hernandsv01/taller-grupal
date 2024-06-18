#ifndef LOBBY_MANAGER_H
#define LOBBY_MANAGER_H

#include <algorithm>
#include <vector>

#include "../../common/library/socket.h"
#include "../../common/library/thread.h"
#include "../../common/map/map.h"
#include "../Game.h"
#include "../game_pool_monitor.h"
#include "lobby_protocol.h"

class LobbyManager : public Thread {
   private:
    LobbyProtocol protocol;
    bool is_running;
    GamePoolMonitor& game_pool;
    std::vector<Map> map_list;

    uint8_t create_game(GameMatch& match_info);

    std::pair<uint16_t, std::string> join_game(uint8_t game_id);

    void sendSocketOfClientToGame(uint8_t game_id, uint16_t player_id,
                                  LobbyProtocol& client);

   public:
    LobbyManager(Socket socket, GamePoolMonitor& game_pool_monitor);

    void run() override;

    void process_command(MessageType command);

    void kill();
};

#endif  // LOBBY_MANAGER_H