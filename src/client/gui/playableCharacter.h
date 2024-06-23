#ifndef PLAYABLE_CHARACTER
#define PLAYABLE_CHARACTER

#include <SDL2pp/SDL2pp.hh>
#include <memory>
#include <string>

#include "entityGame.h"

typedef std::shared_ptr<SDL2pp::Texture> SharedTexturePtr;

class PlayableCharacter : public Entity2 {
   private:
    std::string characterType;
    std::string state;
    int score;
    int health;
    int ammoQuantity;

    SharedTexturePtr weaponTexture;
    int weaponSpriteNumber;

    SharedTexturePtr stateTexture;
    int actualSpriteNumber;
    int spriteSize;
    int spriteLenght;

    SharedTexturePtr hudTexture;
    int hudSpriteNumber;
    int hudSpriteSize;
    int hudSpriteLenght;

    void showHealth(SDL2pp::Renderer &renderer, const int &windowHeight);
    void showScore(SDL2pp::Renderer &renderer);
    void showAmmoQuantity(SDL2pp::Renderer &renderer, const int &windowHeight);

    void showNumber(SDL2pp::Renderer &renderer, const int &number,
                    const int &quantity, const int &initialPos,
                    const int &initialPosY);

   public:
    PlayableCharacter(const std::string &type);

    virtual void renderize(SDL2pp::Renderer &renderer, const int &xRef,
                           const int &yRef, const int &xCenter,
                           const int &yCenter) override;

    void showHud(SDL2pp::Renderer &renderer, const int &windowWidth,
                 const int &windowHeight);

    void updateHealth(const int &newHealthPoint);

    virtual void renderMainPj(SDL2pp::Renderer &renderer, const int &xCenter,
                              const int &yCenter);

    void updateScore(const int &newScore);

    void updateState(const std::string &newState);
};

#endif
