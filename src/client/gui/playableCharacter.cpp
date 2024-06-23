#include "playableCharacter.h"

#include "textureManager.h"

#define STAND "Stand"
#define INITIALHEALTH 10
#define INITIALSCORE 0
#define INITIALBULLETQUANTITY 10

PlayableCharacter::PlayableCharacter(const std::string &type)
    : characterType(type),
      state(STAND),
      Score(INITIALSCORE),
      health(INITIALHEALTH),

      stateTexture(TextureManager::getTexture(characterType + state)),
      spriteSize(stateTexture->GetHeight()),
      spriteLenght(stateTexture->GetWidth() / spriteSize) {}

void PlayableCharacter::updateHealth(const int &newHealthPoint) {
    health = newHealthPoint;
}

void PlayableCharacter::updateScore(const int &newScore) { Score = newScore; }

void PlayableCharacter::updateState(const std::string &newState) {
    std::string newTexture;
    if (newState != state) {
        actualSpriteNumber = 0;
        stateTexture = TextureManager::getTexture(characterType + newState);
        spriteSize = stateTexture->GetHeight();
        spriteLenght = stateTexture->GetWidth() / spriteSize;
    }
}

void PlayableCharacter::renderize(SDL2pp::Renderer &renderer, const int &xRef,
                                  const int &yRef, const int &xCenterWnd,
                                  const int &yCenterWnd) {
    int xPosInRender = positionX - xRef + xCenterWnd;
    int yPosInRender = (positionY - yRef + yCenterWnd) * (-1) + yCenterWnd * 2;
    int turntoLeft = (isRight) ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;

    renderer.Copy(
        *stateTexture,
        SDL2pp::Rect(spriteSize * actualSpriteNumber, 0, spriteSize,
                     spriteSize),
        SDL2pp::Rect(xPosInRender - spriteSize / 2, (yPosInRender - spriteSize),
                     spriteSize, spriteSize),
        0.0, SDL2pp::NullOpt, turntoLeft);

    actualSpriteNumber = (actualSpriteNumber + 1) % spriteLenght;
}

void PlayableCharacter::renderMainPj(SDL2pp::Renderer &renderer,
                                     const int &xCenter, const int &yCenter) {
    renderize(renderer, positionX, positionY, xCenter, yCenter);
}
