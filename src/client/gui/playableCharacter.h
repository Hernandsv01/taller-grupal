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

    SDL2pp::Texture *weaponTexture;
    int weaponSpriteNumber;

    SharedTexturePtr stateTexture;
    int spriteSize;
    int spriteLenght;

    SharedTexturePtr hudTexture;
    int hudSpriteNumber;
    int hudSpriteSize;
    int hudSpriteLenght;

    void showHealth(SDL2pp::Renderer &renderer, const int &windowHeight,
                    uint32_t tick);
    void showScore(SDL2pp::Renderer &renderer);
    void showAmmoQuantity(SDL2pp::Renderer &renderer, const int &windowHeight,
                            const int &windowWidth, uint32_t tick);
    void showRemainingTime(SDL2pp::Renderer &renderer, const int &windowWidth,
                           const int &totalSeconds);

    void showNumber(SDL2pp::Renderer &renderer, const int &number,
                    const int &quantity, const int &initialPos,
                    const int &initialPosY);

   public:
    PlayableCharacter(const std::string &type);

    virtual void renderize(SDL2pp::Renderer &renderer, const int &xRef,
                           const int &yRef, uint32_t tick) override;

    void showHud(SDL2pp::Renderer &renderer, const int &windowWidth,
                 const int &windowHeight, const int &seconds, uint32_t tick);

    virtual void updateHealth(const int &newHealthPoint) override;

    virtual void updateScore(const int &newScore);

    virtual void updateState(const std::string &newState) override;

    virtual void updateWeapon(const std::string &newState) override;

    virtual void updateAmmoQuantity(const int &ammoQuantity) override;

    int getHealth() const;

    virtual bool isPlayer() override;

    std::string getCharacterType() const;

    int getScore() const;
};

#endif
