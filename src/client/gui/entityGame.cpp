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
                        const int &yRef, uint32_t tick) {
    auto *current_texture = TextureManager::getEntityTexture(texture);

    renderizeWithTexture(renderer, *current_texture, xRef, yRef, tick);
}

void Entity2::renderizeWithTexture(SDL2pp::Renderer &renderer,
                                   SDL2pp::Texture &texture, const int &xRef,
                                   const int &yRef, uint32_t tick) {
    int xPosInRender = positionX - xRef;
    int yPosInRender = positionY - yRef;
    int turntoLeft = (isRight) ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;

    int texture_size = texture.GetHeight();

    int num_animation_frames = texture.GetWidth() / texture_size;

    renderer.Copy(
        texture,
        SDL2pp::Rect(texture_size * actualSpriteNumber, 0, texture_size,
                     texture_size),
        SDL2pp::Rect(xPosInRender - texture_size / 2,
                     (yPosInRender - texture_size), texture_size, texture_size),
        0.0, SDL2pp::NullOpt, turntoLeft);

    // Cuadrado Debugging. Muestra la posicion real de la entidad.
    // renderer.FillRect(SDL2pp::Rect(xPosInRender - 2, yPosInRender - 2, 5,
    // 5));

    int advance_frame = (tick % ANIMATION_RATE == 0);

    if (!loopAnimation && actualSpriteNumber == num_animation_frames - 1) {
        advance_frame = 0;
    }

    actualSpriteNumber =
        (actualSpriteNumber + advance_frame) % num_animation_frames;
}

bool Entity2::isPlayer() { return false; }