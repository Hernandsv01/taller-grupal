#include "entityGame.h"

#include "textureManager.h"

Entity2::Entity2() : positionX(0), positionY(0), isRight(true) {}

Entity2::Entity2(std::string texture)
    : positionX(0), positionY(0), isRight(true), texture(texture) {}

void Entity2::updatePosition(const int &posX, const int &posY) {
    positionX = posX;
    positionY = posY;
}

int Entity2::getPosX() const { return positionX; }

int Entity2::getPosY() const { return positionY; }

void Entity2::renderize(SDL2pp::Renderer &renderer, const int &xRef,
                        const int &yRef, const int &xCenter,
                        const int &yCenter) {
    int xPosInRender = positionX - xRef + xCenter;
    int yPosInRender = (positionY - yRef + yCenter) * (-1) + yCenter * 2;
    int turntoLeft = (isRight) ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;

    auto current_texture = TextureManager::getEntityTexture(texture);

    int texture_size = current_texture->GetHeight();

    int num_animation_frames = current_texture->GetWidth() / texture_size;

    renderer.Copy(
        (*current_texture),
        SDL2pp::Rect(texture_size * actualSpriteNumber, 0, texture_size,
                     texture_size),
        SDL2pp::Rect(xPosInRender - texture_size / 2,
                     (yPosInRender - texture_size), texture_size, texture_size),
        0.0, SDL2pp::NullOpt, turntoLeft);

    actualSpriteNumber = (actualSpriteNumber + 1) % num_animation_frames;
}