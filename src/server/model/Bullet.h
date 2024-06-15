#ifndef BULLET_H
#define BULLET_H

#include "Dynamic_entity.h"

#define BULLET_HEIGHT 1
#define BULLET_WIDTH 1

class Bullet : public Dynamic_entity {
public:
    Bullet(int id, float x_spawn, float y_spawn, float x_speed, float y_speed)
            : Dynamic_entity(id, x_spawn, y_spawn, BULLET_WIDTH, BULLET_HEIGHT, x_speed, y_speed, 0, false, 5, false, 0) {};

    std::vector<Update> tick(Map map,
                             std::vector<std::unique_ptr<Dynamic_entity>>* entity_pool) override {
        std::vector<Update> updates;

        // validar movimiento en X contra mapa
        if (vel_x != 0) {
            x_pos += vel_x;

            bool collides = false;

            for (float i = this->x_min(); i < this->x_max() && !collides; i++) {
                for (float j = this->y_min(); j < this->y_max() && !collides; j++) {
                    Collision collision = map.get_block_collision({static_cast<uint8_t>(std::floor(i)),static_cast<uint8_t>(std::floor(j))});
                    if (collision != Collision::Air) {
                        collides = true;
                    }
                }
            }

            if (collides) {
                Update update{static_cast<uint16_t>(id),
                              Updateables::DESTROYED,
                              static_cast<uint32_t>(0)};
                updates.push_back(update);
                // TODO: delete bullet from entity pool
                return updates;
            } else {
                Update update{static_cast<uint16_t>(id),
                              Updateables::POSITION_X,
                              static_cast<uint32_t>(x_pos)};
                updates.push_back(update);
            }
        }

        // validar movimiento en Y contra mapa
        if (vel_y != 0) {
            y_pos += vel_y;

            bool collides = false;

            for (float i = this->x_min(); i < this->x_max() && !collides; i++) {
                for (float j = this->y_min(); j < this->y_max() && !collides; j++) {
                    Collision collision = map.get_block_collision({static_cast<uint8_t>(std::floor(i)),static_cast<uint8_t>(std::floor(j))});
                    if (collision != Collision::Air) {
                        collides = true;
                    }
                }
            }

            if (collides) {
                Update update{static_cast<uint16_t>(id),
                              Updateables::DESTROYED,
                              static_cast<uint32_t>(0)};
                updates.push_back(update);
                // TODO: delete bullet from entity pool
                return updates;
            } else {
                Update update{static_cast<uint16_t>(id),
                              Updateables::POSITION_Y,
                              static_cast<uint32_t>(y_pos)};
                updates.push_back(update);
            }
        }

        // validar contacto con otras entidades
        for (const std::unique_ptr<Dynamic_entity>& other : *entity_pool) {
            if (!this->colisiona_con(*other)) {
                continue;
            }

            if (other->is_entity_damageable()) {
                bool is_dead = other->deal_damage(other->get_damage_dealt());

                if (is_dead) {
                    Update update{static_cast<uint16_t>(other->get_id()),
                                  Updateables::DESTROYED,
                                  static_cast<uint32_t>(0)};
                    updates.push_back(update);
                } else {
                    Update update{static_cast<uint16_t>(other->get_id()),
                                  Updateables::HEALTH,
                                  static_cast<uint32_t>(health)};
                    updates.push_back(update);
                }

                Update update{static_cast<uint16_t>(this->id),
                              Updateables::DESTROYED,
                              static_cast<uint32_t>(0)};
                updates.push_back(update);
            }
        }

        return updates;
    }
};

#endif //BULLET_H
