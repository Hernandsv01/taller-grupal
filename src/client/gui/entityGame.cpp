#include "entityGame.h"

Entity2::Entity2() :
    positionX(0), positionY(0),
    direction(true) {
}

void Entity2::updatePosition(const int &posX, const int &posY) {
    positionX = posX;
    positionY = posY;
}

int Entity2::getPosX() const {
    return positionX;
}

int Entity2::getPosY() const {
    return positionY;
}
