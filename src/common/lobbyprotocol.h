#ifndef LOBBYPROTOCOL_H
#define LOBBYPROTOCOL_H

#include <arpa/inet.h>
#include <string.h>

#include <vector>

#include "library/socket.h"

typedef uint8_t match_id;

struct GameMatch {
    match_id id;
    std::string name;
    std::string map;
}__attribute__((packed));

enum MessageType : uint8_t { CREATE, JOIN, GET_GAMES };

#endif  // LOBBYPROTOCOL_H