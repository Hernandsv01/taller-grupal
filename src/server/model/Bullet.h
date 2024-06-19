#ifndef BULLET_H
#define BULLET_H

#include "Dynamic_entity.h"

#define BULLET_HEIGHT 1
#define BULLET_WIDTH 1

class Bullet : public Dynamic_entity {
public:
    Bullet(int id, float x_spawn, float y_spawn, float x_speed, float y_speed)
            : Dynamic_entity(id, x_spawn, y_spawn, BULLET_WIDTH, BULLET_HEIGHT, x_speed, y_speed, 0, false, 5, false, 0) {};

    std::vector<Update::Update_new> tick(const Map& map,
                             std::vector<std::unique_ptr<Dynamic_entity>>& entity_pool) override {
        std::vector<Update::Update_new> updates;

        // validar movimiento contra mapa
        if (vel_x != 0 || vel_y != 0) {
            x_pos += vel_x;
            y_pos += vel_y;

            for (float i = this->x_min(); i <= this->x_max(); i++) {
                for (float j = this->y_min(); j < this->y_max(); j++) {
                    Collision collision = map.get_block_collision({static_cast<uint8_t>(std::floor(i)),static_cast<uint8_t>(std::floor(j))});
                    if (collision != Collision::Air) {
                        updates.push_back(Update::Update_new::create_delete_entity(id));
                        delete_bullet(entity_pool, id);
                        return updates;
                    }
                }
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
