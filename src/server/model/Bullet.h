#ifndef BULLET_H
#define BULLET_H

#include <algorithm>

#include "Player.h"
#include "Dynamic_entity.h"

#define BULLET_HEIGHT 0.01
#define BULLET_WIDTH 0.01

class Bullet : public Dynamic_entity {
   public:
    Bullet(int id, float x_spawn, float y_spawn, float vel_x, int damage)
        : Dynamic_entity(id, x_spawn, y_spawn, BULLET_WIDTH, BULLET_HEIGHT,
                         vel_x, 0, 0, false, damage, false, 0, true){};

    std::vector<Update::Update_new> tick(
        const Map& map,
        std::vector<std::unique_ptr<Dynamic_entity>>& entity_pool,
        int& next_id) override {
        std::vector<Update::Update_new> updates;

        // validar movimiento contra mapa
        if (vel_x != 0 || vel_y != 0) {
            x_pos += vel_x;
            y_pos += vel_y;

            if (collides_with_map(map)) {
                std::cout << "Se la dió contra el mapa" << std::endl;
                updates.push_back(Update::Update_new::create_delete_entity(id));
                pending_deletion = true;
                return updates;
            }
        }

        // validar contacto con otras entidades
        for (const std::unique_ptr<Dynamic_entity>& other : entity_pool) {
            if (!this->colisiona_con(*other)) {
                continue;
            }

            if (other->is_entity_damageable()) {
                std::cout << "Se la dió contra alguien" << std::endl;
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
                pending_deletion = true;
                return updates;
            }
        }

        updates.push_back(Update::Update_new::create_position(
            static_cast<uint16_t>(id), x_pos, y_pos));

        return updates;
    }
};

#endif  // BULLET_H