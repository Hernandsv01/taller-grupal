#ifndef LOBBY_PROTOCOK_H
#define LOBBY_PROTOCOK_H

#include "../../common/library/socket.h"
#include "../../common/lobbyprotocol.h"
#include "../Game.h"

class LobbyProtocol {
   private:
    Socket socket;
    bool valid = true;

   public:
    explicit LobbyProtocol(Socket socket);

    // recibe comando
    MessageType receive_command();

    //read CREATE
    GameMatch receive_create_match();
    //send CREATE
    void send_created_match(match_id& id);

    //read JOIN
    match_id receive_join_game();
    //send JOIN
    void send_join_game(std::pair<uint16_t, std::string>& joined_info);

    //send GET GAMES
    void send_games(std::vector<GameMatch>& games);

    // extract socket from lobbyProtocol
    Socket extract_socket();

    // devuelve si es valido
    bool is_valid();
};

#endif  //LOBBY_PROTOCOK_H
