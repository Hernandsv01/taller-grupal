#include "entityFactory.h"

#include "../common/Update.h"
#include "playableCharacter.h"

const std::vector<std::string> EntityFactory::subtype_chart = {
    "No_subtype", "Jazz",   "Spaz", "Lori",  "Enemy1",
    "Enemy2",     "Enemy3", "Coin", "Weapon"};

std::shared_ptr<Entity2> EntityFactory::createEntity(const int& type,
                                                     const int& subtype) {
    switch (type) {
        case Update::EntityType::Player:
            return std::make_shared<PlayableCharacter>(subtype_chart[subtype]);
        case Update::EntityType::Bullet:
            return std::make_shared<Entity2>("bullet");
        case Update::EntityType::Enemy:
            return std::make_shared<Entity2>("enemy1");
        default:
            return std::make_shared<Entity2>("placeholder");
    }
}