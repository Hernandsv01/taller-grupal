#include "update_queue.h"
Queue<std::vector<Update>> updates;

void Update_queue::push(std::vector<Update> &update) {
    updates.push(update);
}

void Update_queue::try_push(std::vector<Update> &update) {
    updates.try_push(update);
}

std::vector<Update> Update_queue::pop() {
    return updates.pop();
}

std::vector<Update> Update_queue::try_pop() {
    std::vector<Update> update;
    if(updates.try_pop(update)){
        return update;
    }
    return update;
}

