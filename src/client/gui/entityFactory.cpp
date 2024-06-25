#include "entityFactory.h"

#include "../common/Update.h"
#include "playableCharacter.h"

const std::vector<std::string> EntityFactory::subtype_chart = {
    "no_subtype", "Jazz", "Spaz",   "Lori",  "enemy1", "enemy2",
    "enemy3",     "coin", "carrot",
    "bulletNormal", "bulletLight", "bulletHeavy",  "bulletPower",};

std::shared_ptr<Entity2> EntityFactory::createEntity(const int& type,
                                                     const int& subtype) {
    switch (type) {
        case Update::EntityType::Player:
            switch (subtype) {
            case Update::EntitySubtype::Jazz:
                return std::make_shared<PlayableCharacter>(subtype_chart[1]);
            case Update::EntitySubtype::Lori:
                return std::make_shared<PlayableCharacter>(subtype_chart[3]);
            }
        
        case Update::EntityType::Bullet:
            switch (subtype) {
                case enums_value_update::Ammo_type::NORMAL:
                    return std::make_shared<PlayableCharacter>(subtype_chart[9]);
                case enums_value_update::Ammo_type::LIGHT:
                    return std::make_shared<PlayableCharacter>(subtype_chart[10]);
                case enums_value_update::Ammo_type::HEAVY:
                    return std::make_shared<PlayableCharacter>(subtype_chart[11]);
                case enums_value_update::Ammo_type::POWER:
                    return std::make_shared<PlayableCharacter>(subtype_chart[12]);
            }
        case Update::EntityType::Enemy:
        case Update::EntityType::Item: {
            return std::make_shared<Entity2>(subtype_chart[subtype]);
        }
        default:
            return std::make_shared<Entity2>("placeholder");
    }
}