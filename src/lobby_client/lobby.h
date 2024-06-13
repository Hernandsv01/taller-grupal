#ifndef LOBBY_H
#define LOBBY_H

#include <optional>
#include <stdexcept>

#include "protocololobby.h"

class Lobby {
    LobbyProtocol* lobbyProtocol = nullptr;
    bool hasConnectedToMatch = false;
    uint16_t assigned_player_id = 0;
    std::string assigned_map_name = "";

    void checkProtocolOrError(const char* error) const;

    void checkIsConnectedToMatch() const;

   public:
    Lobby();

    int exec();

    void connectToServer(const std::string& ip, const std::string& port);
    void desconnectFromServer();

    std::vector<GameMatch> getServerMatches();

    void connectToMatch(u_int16_t id);

    match_id createMatch(const std::string& selected_map,
                         const std::string& matchName);

    bool isConnectedToMatch() const;

    Socket getMatchConnection();

    std::pair<uint16_t, std::string> getPlayerIdAndMapName();

    Lobby(const Lobby&) = delete;
    Lobby& operator=(const Lobby&) = delete;

    ~Lobby();
};

#endif  // LOBBY_H
