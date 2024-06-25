#ifndef BULLET_H
#define BULLET_H

#include <algorithm>

#include "Dynamic_entity.h"
#include "Enemy.h"

#define BULLET_HEIGHT 0.35
#define BULLET_WIDTH 0.25
#define KILLING_POINTS 5

class Bullet : public Dynamic_entity {
private:
    int shooter_id;
public:
    Bullet(int id, float x_spawn, float y_spawn, float vel_x, int damage, int shooter_id)
        : Dynamic_entity(id, x_spawn, y_spawn, BULLET_WIDTH, BULLET_HEIGHT,
                         vel_x, 0, 0, 0, false, damage, false, 0, true), shooter_id(shooter_id) {};

    std::vector<Update::Update_new> tick(
        const Map& map,
        std::vector<std::unique_ptr<Dynamic_entity>>& entity_pool,
        int& next_id) override {
        std::vector<Update::Update_new> updates;
        std::vector<Update::Update_new> death_updates;

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
                bool is_dead = other->deal_damage(get_damage_dealt());

                if (is_dead) {
                    std::cout << "Entidad muerta!" << std::endl;
                    death_updates = other->handle_death(entity_pool, next_id);
                    updates.insert(updates.end(), death_updates.begin(), death_updates.end());

                    int shooter_position = findEntityPositionById(entity_pool, shooter_id);
                    entity_pool[shooter_position]->increase_points(KILLING_POINTS);
                    if (entity_pool[shooter_position]->get_points() != -1) {
                        std::cout << "Pusheando update de points: " << entity_pool[shooter_position]->get_points() << std::endl;
                        updates.push_back(Update::Update_new::create_value(
                                static_cast<uint16_t>(shooter_id),
                                Update::UpdateType::Score,
                                static_cast<uint8_t>(entity_pool[shooter_position]->get_points())));
                    }
                } else {
                    updates.push_back(Update::Update_new::create_value(
                            static_cast<uint16_t>(other->get_id()),
                            Update::UpdateType::Health,
                            static_cast<uint8_t>(other->get_health())));
                }


                updates.push_back(Update::Update_new::create_delete_entity(id));
                pending_deletion = true;
                return updates;
            }
        }

        auto [x_client, y_client] = get_position_for_client();

        updates.push_back(Update::Update_new::create_position(
            static_cast<uint16_t>(id), x_client, y_client));

        return updates;
    }

    int findEntityPositionById(std::vector<std::unique_ptr<Dynamic_entity>>& entities, int entity_id) {
        auto it = std::find_if(
                entities.begin(), entities.end(),
                [entity_id](const std::unique_ptr<Dynamic_entity>& entity) {
                    return entity->get_id() == entity_id;
                });
        if (it != entities.end()) {
            return std::distance(entities.begin(), it);
        }
        return -1;
    }

    std::vector<Update::Update_new> handle_death(std::vector<std::unique_ptr<Dynamic_entity>>& entity_pool, int& next_id) override {
        std::vector<Update::Update_new> updates;
        updates.push_back(Update::Update_new::create_delete_entity(id));
        set_pending_deletion();
        return updates;
    }

    void increase_points(int more_points) { };
    virtual int get_points() { return -1; };
};

#endif  // BULLET_H