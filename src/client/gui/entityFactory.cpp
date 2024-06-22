#include "entityFactory.h"
#include "playableCharacter.h"

#define PLAYER 0
#define ENEMY 2

std::shared_ptr<Entity2> EntityFactory::createEntity(const int& type, const int& subtype) {
    //if (type == PLAYER) {
        return std::make_shared<PlayableCharacter>("Jazz");
    //} else if (type == ENEMY) {
    //    return std::make_unique<Enemy>("jazz");
    //}
}