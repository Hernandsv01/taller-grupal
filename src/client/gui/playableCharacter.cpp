#include "playableCharacter.h"

#include <cmath>

#include "textureManager.h"

#define HUD "Hud"
#define STAND "Stand"
#define DEFAULTWEAPON "weaponNormal"
#define INF "infinity"
#define SPACETOBORDER 5
#define MAXSCOREDIGIT 7
#define SECONDINMIN 60

#define INITIALHEALTH 0
#define INITIALSCORE 0
#define INITIALBULLETQUANTITY 10

PlayableCharacter::PlayableCharacter(const std::string &type)
    : characterType(type),
      state(STAND),
      score(INITIALSCORE),
      health(INITIALHEALTH),
      ammoQuantity(-1),

      weaponTexture(
          TextureManager::getEntityTexture(std::string(DEFAULTWEAPON))),
      weaponSpriteNumber(0),

      stateTexture(TextureManager::getTexture(characterType + state)),
      actualSpriteNumber(0),
      spriteSize(stateTexture->GetHeight()),
      spriteLenght(stateTexture->GetWidth() / spriteSize),

      hudTexture(TextureManager::getTexture(characterType + HUD)),
      hudSpriteNumber(0),
      hudSpriteSize(hudTexture->GetHeight()),
      hudSpriteLenght(hudTexture->GetWidth() / hudSpriteSize) {}

void PlayableCharacter::updateHealth(const int &newHealthPoint) {
    health = newHealthPoint;
}

void PlayableCharacter::updateScore(const int &newScore) { score = newScore; }

void PlayableCharacter::updateState(const std::string &newState) {
    actualSpriteNumber = 0;
    stateTexture = TextureManager::getTexture(characterType + newState);
    spriteSize = stateTexture->GetHeight();
    spriteLenght = stateTexture->GetWidth() / spriteSize;
}

void PlayableCharacter::updateWeapon(const std::string &newWeapon) {
    weaponTexture = TextureManager::getEntityTexture(newWeapon);
    weaponSpriteNumber = 0;
}

void PlayableCharacter::updateAmmoQuantity(const int &newAmmoQuantity) {
    ammoQuantity = newAmmoQuantity;
}

void PlayableCharacter::renderize(SDL2pp::Renderer &renderer, const int &xRef,
                                  const int &yRef) {
    int xPosInRender = positionX - xRef;
    int yPosInRender = positionY - yRef;
    int turntoLeft = (isRight) ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;

    renderer.Copy(
        *stateTexture,
        SDL2pp::Rect(spriteSize * actualSpriteNumber, 0, spriteSize,
                     spriteSize),
        SDL2pp::Rect(xPosInRender - spriteSize / 2, (yPosInRender - spriteSize),
                     spriteSize, spriteSize),
        0.0, SDL2pp::NullOpt, turntoLeft);

    renderer.FillRect(SDL2pp::Rect(xPosInRender - 2, yPosInRender - 2, 5, 5));

    actualSpriteNumber = (actualSpriteNumber + 1) % spriteLenght;
}

void PlayableCharacter::renderMainPj(SDL2pp::Renderer &renderer,
                                     const int &xRef, const int &yRef) {
    renderize(renderer, xRef, yRef);
}

void PlayableCharacter::showHud(SDL2pp::Renderer &renderer,
                                const int &windowWidth, const int &windowHeight,
                                const int &seconds) {
    showHealth(renderer, windowHeight);
    showScore(renderer);
    showAmmoQuantity(renderer, windowHeight);
    showRemainingTime(renderer, windowWidth, seconds);
}

void PlayableCharacter::showAmmoQuantity(SDL2pp::Renderer &renderer,
                                         const int &windowHeight) {
    int textureSize = weaponTexture->GetHeight();
    int textureLength = weaponTexture->GetWidth() / textureSize;
    renderer.Copy(*weaponTexture,
                  SDL2pp::Rect(textureSize * weaponSpriteNumber, 0, textureSize,
                               textureSize),
                  SDL2pp::Rect(560, windowHeight - textureSize - SPACETOBORDER,
                               textureSize, textureSize));

    weaponSpriteNumber = (weaponSpriteNumber + 1) % textureLength;

    if (ammoQuantity < 0) {
        SharedTexturePtr infinityTexture = TextureManager::getTexture(INF);
        int infinityWidth = infinityTexture->GetWidth();
        int infinityHeight = infinityTexture->GetHeight();
        renderer.Copy(
            *infinityTexture, SDL2pp::NullOpt,
            SDL2pp::Rect(textureSize + 560,
                         windowHeight - infinityHeight - SPACETOBORDER,
                         infinityWidth, infinityHeight));
    } else {
        showNumber(renderer, ammoQuantity,
                   std::to_string(ammoQuantity).length(), textureSize + 560,
                   windowHeight - textureSize - SPACETOBORDER);
    }
}

void PlayableCharacter::showHealth(SDL2pp::Renderer &renderer,
                                   const int &windowHeight) {
    renderer.Copy(*hudTexture,
                  SDL2pp::Rect(hudSpriteSize * hudSpriteNumber, 0,
                               hudSpriteSize, hudSpriteSize),
                  SDL2pp::Rect(SPACETOBORDER, windowHeight - hudSpriteSize,
                               hudSpriteSize, hudSpriteSize));

    hudSpriteNumber = (hudSpriteNumber + 1) % hudSpriteLenght;

    showNumber(renderer, health, std::to_string(health).length(),
               hudSpriteSize + SPACETOBORDER, windowHeight - hudSpriteSize);
}

void PlayableCharacter::showScore(SDL2pp::Renderer &renderer) {
    showNumber(renderer, score, MAXSCOREDIGIT, SPACETOBORDER, SPACETOBORDER);
}

void PlayableCharacter::showRemainingTime(SDL2pp::Renderer &renderer,
                                          const int &windowWidth,
                                          const int &totalSeconds) {
    int minutes = totalSeconds / SECONDINMIN;
    int seconds = totalSeconds % SECONDINMIN;
    // Show minutes
    showNumber(renderer, minutes, 2, 560, SPACETOBORDER);

    // Show ":"
    SharedTexturePtr separatorTexture = TextureManager::getTexture("separator");
    int separatorWidth = separatorTexture->GetWidth();
    int separatorHeight = separatorTexture->GetHeight();
    renderer.Copy(
        *separatorTexture, SDL2pp::NullOpt,
        SDL2pp::Rect(595, SPACETOBORDER, separatorWidth, separatorHeight));

    // Show seconds
    showNumber(renderer, seconds, 2, 600 + separatorWidth, SPACETOBORDER);
}

void PlayableCharacter::showNumber(SDL2pp::Renderer &renderer,
                                   const int &number, const int &quantity,
                                   const int &initialPosX,
                                   const int &initialPosY) {
    int digitToShow = pow(10, quantity - 1);
    int numberRemaining = number;
    int posInScreen = initialPosX;
    for (int i = digitToShow; i > 0; i /= 10) {
        int number = numberRemaining / i;
        numberRemaining = numberRemaining % i;
        SharedTexturePtr numberTexture =
            TextureManager::getTexture(std::to_string(number));

        int numberWidth = numberTexture->GetWidth();
        int numberHeight = numberTexture->GetHeight();
        renderer.Copy(
            *numberTexture, SDL2pp::NullOpt,
            SDL2pp::Rect(posInScreen, initialPosY, numberWidth, numberHeight));

        posInScreen += numberTexture->GetWidth();
    }
}

bool PlayableCharacter::isPlayer() { return true; }

std::string PlayableCharacter::getCharacterType() const {
    return characterType;
}

int PlayableCharacter::getScore() const { return score; }