#ifndef PLAYABLE_CHARACTER
#define PLAYABLE_CHARACTER

#include <SDL2pp/SDL2pp.hh>
#include <memory>
#include <string>

#include "entityGame.h"

class PlayableCharacter : public Entity2 {
   private:
    std::string characterType;
    std::string state;
    int Score;
    int health;

    std::shared_ptr<SDL2pp::Texture> stateTexture;
    int spriteSize;
    int spriteLenght;

   public:
    PlayableCharacter(const std::string &type);

    virtual void renderize(SDL2pp::Renderer &renderer, const int &xRef,
                           const int &yRef, const int &xCenter,
                           const int &yCenter) override;

    virtual void renderMainPj(SDL2pp::Renderer &renderer, const int &xCenter,
                              const int &yCenter);

    void updateHealth(const int &newHealthPoint);

    void updateScore(const int &newScore);

    void updateState(const std::string &newState);
};

#endif
