#include "entityFactory.h"

#include "../common/Update.h"
#include "playableCharacter.h"

const std::vector<std::string> EntityFactory::subtype_chart = {
    "no_subtype", "Jazz", "Spaz",   "Lori",  "enemy1", "enemy2",
    "enemy3",     "coin", "carrot", "light", "heavy",  "power"};

std::shared_ptr<Entity2> EntityFactory::createEntity(const int& type,
                                                     const int& subtype) {
    switch (type) {
        case Update::EntityType::Player:
            return std::make_shared<PlayableCharacter>(subtype_chart[subtype]);
        case Update::EntityType::Bullet:
            return std::make_shared<Entity2>("bullet");
        case Update::EntityType::Enemy:
        case Update::EntityType::Item: {
            return std::make_shared<Entity2>(subtype_chart[subtype]);
        }
        default:
            return std::make_shared<Entity2>("placeholder");
    }
}