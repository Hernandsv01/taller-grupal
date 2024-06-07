#include "update_queue.h"
Queue<std::vector<Update>> Update_queue::updates;

void Update_queue::push(std::vector<Update> &update) { updates.push(update); }

void Update_queue::try_push(std::vector<Update> &update) {
    updates.try_push(update);
}

std::vector<Update> Update_queue::pop() { return updates.pop(); }

std::vector<Update> Update_queue::try_pop() {
    std::vector<Update> update;
    if (updates.try_pop(update)) {
        return update;
    }
    return update;
}

std::vector<Update> Update_queue::try_pop_all() {
    std::vector<Update> all_updates;
    std::vector<Update> update_one_tick;

    // Popeo de a uno los vectores de updates, y voy insertando al final del
    // vector de todos los updates existentes.

    // Cuando ya no quede ningun vec<update> en la cola, este ultimo tendra
    // tamaño 0 por lo que se corta el ciclo.
    do {
        update_one_tick = Update_queue::try_pop();
        all_updates.insert(all_updates.end(), update_one_tick.begin(),
                           update_one_tick.end());
    } while (update_one_tick.size() > 0);

    return all_updates;
}
