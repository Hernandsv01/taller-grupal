#include "playableCharacter.h"
#include "textureManager.h"

#define HUD "Hud"
#define STAND "Stand"
#define INITIALHEALTH 10
#define INITIALSCORE 0
#define INITIALBULLETQUANTITY 10

PlayableCharacter::PlayableCharacter(const std::string& type) :
    characterType(type),
    isRight(true),
    state(STAND),
    score(INITIALSCORE),
    health(INITIALHEALTH),

    stateTexture(TextureManager::getTexture(characterType+state)),
    actualSpriteNumber(0),
    spriteSize(stateTexture->GetHeight()),
    spriteLenght(stateTexture->GetWidth() / spriteSize),
    
    hudTexture(TextureManager::getTexture(characterType+HUD)),
    hudSpriteNumber(0),
    hudSpriteSize(hudTexture->GetHeight()),
    hudSpriteLenght(hudTexture->GetWidth() / hudSpriteSize) {
}

void PlayableCharacter::updateHealth(const int &newHealthPoint) {
    health = newHealthPoint;
}

void PlayableCharacter::updateScore(const int &newScore) {
    score = newScore;
}

void PlayableCharacter::updateState(const std::string &newState) {
    std::string newTexture;
    if (newState != state) {
        actualSpriteNumber = 0;
        stateTexture = TextureManager::getTexture(characterType+newState);
        spriteSize = stateTexture->GetHeight();
        spriteLenght = stateTexture->GetWidth() / spriteSize;
    }
}

void PlayableCharacter::renderize(SDL2pp::Renderer &renderer,
                            const int &xRef, const int &yRef,
                            const int &xCenterWnd, const int &yCenterWnd) {
    
    int xPosInRender = positionX - xRef + xCenterWnd;
    int yPosInRender = (positionY - yRef + yCenterWnd) *(-1) + yCenterWnd *2;
    int turntoLeft = (isRight) ? SDL_FLIP_NONE: SDL_FLIP_HORIZONTAL;

    renderer.Copy(
                *stateTexture,
                SDL2pp::Rect(spriteSize* actualSpriteNumber, 0,
                                    spriteSize, spriteSize),
                SDL2pp::Rect(xPosInRender- spriteSize/2, (yPosInRender - spriteSize),
                                    spriteSize, spriteSize),
                0.0,
                SDL2pp::NullOpt,
                turntoLeft);

    actualSpriteNumber = (actualSpriteNumber + 1) % spriteLenght;
}

void PlayableCharacter::renderMainPj(SDL2pp::Renderer &renderer,
                                const int &xCenter, const int &yCenter) {
    renderize(renderer, positionX, positionY, xCenter, yCenter);
}

void PlayableCharacter::showHud(SDL2pp::Renderer &renderer,
                    const int &windowWidth, const int &windowHeight) {
    
    renderer.Copy(*hudTexture,
                SDL2pp::Rect(hudSpriteSize* hudSpriteNumber, 0,
                                    hudSpriteSize, hudSpriteSize),
                SDL2pp::Rect(5, windowHeight*0.72,
                                    hudSpriteSize, hudSpriteSize));

    hudSpriteNumber = (hudSpriteNumber + 1) % hudSpriteLenght;

    showScore(renderer);
}

void PlayableCharacter::showScore(SDL2pp::Renderer &renderer) {
    int numberPos = 5;
    int scoreRemainig = score;
    for (int i = 1000000; i>0; i /= 10) {
        int number = scoreRemainig/i;
        scoreRemainig = scoreRemainig % i;
        std::shared_ptr<SDL2pp::Texture> numberTexture = 
                            TextureManager::getTexture(std::to_string(number));
    
        int numberWidth = numberTexture->GetWidth();
        int numberHeight = numberTexture->GetHeight();
        renderer.Copy(*numberTexture,
                SDL2pp::NullOpt,
                SDL2pp::Rect(numberPos, 5, numberWidth, numberHeight));

        numberPos += numberTexture->GetWidth();
    }

}

void PlayableCharacter::updateDirection(bool &isFacingRight) {
    isRight = isFacingRight;
}
