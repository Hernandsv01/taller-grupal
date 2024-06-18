#ifndef UPDATE_QUEUE_H
#define UPDATE_QUEUE_H

#include <sstream>
#include <vector>

#include "../common/Update.h"
#include "../common/library/queue.h"

class Update_queue {
   private:
    static Queue<std::vector<Update::Update_new>> updates;

   public:
    static void push(std::vector<Update::Update_new> &update);
    static void try_push(std::vector<Update::Update_new> &update);
    static std::vector<Update::Update_new> pop();
    static std::vector<Update::Update_new> try_pop();
    static std::vector<Update::Update_new> try_pop_all();
};

#endif  // UPDATE_MONITOR_H
