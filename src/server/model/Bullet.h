#ifndef BULLET_H
#define BULLET_H

#include "Dynamic_entity.h"
#include <algorithm>

#define BULLET_HEIGHT 1
#define BULLET_WIDTH 1

class Bullet : public Dynamic_entity {
public:
    Bullet(int id, float x_spawn, float y_spawn, float vel_x, int damage)
            : Dynamic_entity(id, x_spawn, y_spawn, BULLET_WIDTH, BULLET_HEIGHT, vel_x, 0, 0, false, damage, false, 0, true, true) {};

    std::vector<Update::Update_new> tick(const Map& map,
                             std::vector<std::unique_ptr<Dynamic_entity>>& entity_pool, int& next_id) override {
        std::vector<Update::Update_new> updates;

        // validar movimiento contra mapa
        if (vel_x != 0 || vel_y != 0) {
            x_pos += vel_x;
            y_pos += vel_y;

            if (collides_with_map(map)) {
                updates.push_back(Update::Update_new::create_delete_entity(id));
                delete_bullet(entity_pool, id);
                return updates;
            }
        }

        // validar contacto con otras entidades
        for (const std::unique_ptr<Dynamic_entity>& other : entity_pool) {
            if (!this->colisiona_con(*other)) {
                continue;
            }

            if (other->is_entity_damageable()) {
                bool is_dead = other->deal_damage(get_damage_dealt());

                if (is_dead) {
                    updates.push_back(Update::Update_new::create_value(
                            static_cast<uint16_t>(other->get_id()),
                            Update::UpdateType::State,
                            enums_value_update::Player_State_Enum::Dead));
                } else {
                    updates.push_back(Update::Update_new::create_value(
                            static_cast<uint16_t>(other->get_id()),
                            Update::UpdateType::Health,
                            static_cast<uint8_t>(health)));
                }

                updates.push_back(Update::Update_new::create_delete_entity(id));
                return updates;
            }
        }

        updates.push_back(Update::Update_new::create_position(
                static_cast<uint16_t>(id),
                x_pos,
                y_pos));

        return updates;
    }

    void delete_bullet(std::vector<std::unique_ptr<Dynamic_entity>>& entity_pool, int id) {
        auto it = std::find_if(entity_pool.begin(), entity_pool.end(),
                               [id](const std::unique_ptr<Dynamic_entity>& entity) {
                                   return entity->get_id() == id;
                               });
        if (it != entity_pool.end()) {
            entity_pool.erase(it);
        }
    }
};

#endif //BULLET_H
