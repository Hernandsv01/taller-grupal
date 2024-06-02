#ifndef ACTIONMESSAGE_H
#define ACTIONMESSAGE_H

#include <sstream>

struct ActionMessage{
    std::uint8_t action;
}__attribute__((packed));

#endif //ACTIONMESSAGE_H
