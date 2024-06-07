#ifndef UPDATE_QUEUE_H
#define UPDATE_QUEUE_H

#include <sstream>
#include <vector>

#include "../common/Update.h"
#include "../common/library/queue.h"

class Update_queue {
   private:
    static Queue<std::vector<Update>> updates;

   public:
    static void push(std::vector<Update>& update);
    static void try_push(std::vector<Update>& update);
    static std::vector<Update> pop();
    static std::vector<Update> try_pop();
    static std::vector<Update> try_pop_all();
};

#endif  // UPDATE_MONITOR_H
