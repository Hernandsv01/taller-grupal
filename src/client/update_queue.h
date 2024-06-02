#ifndef UPDATE_MONITOR_H
#define UPDATE_MONITOR_H

#include "../common/library/queue.h"
#include "../common/Update.h"
#include <sstream>
#include <vector>

class Update_queue{
private:
    static Queue<std::vector<Update>> updates;
public:
    static void push(std::vector<Update>& update);
    static void try_push(std::vector<Update>& update);
    static std::vector<Update> pop();
    static std::vector<Update> try_pop();
};

#endif //UPDATE_MONITOR_H
