#ifndef PLAYABLE_CHARACTER
#define PLAYABLE_CHARACTER

#include <memory>
#include <string>
#include "entityGame.h"
#include <SDL2pp/SDL2pp.hh>

class PlayableCharacter : public Entity2 {
    private:
        std::string characterType;
        bool isRight;
        std::string state;
        int score;
        int health;

        std::shared_ptr<SDL2pp::Texture> stateTexture;
        int actualSpriteNumber;
        int spriteSize;
        int spriteLenght;

        std::shared_ptr<SDL2pp::Texture> hudTexture;
        int hudSpriteNumber;
        int hudSpriteSize;
        int hudSpriteLenght;


        void showScore(SDL2pp::Renderer &renderer);
        
    public:
        PlayableCharacter(const std::string& type);

        virtual void renderize(SDL2pp::Renderer &renderer,
                            const int &xRef, const int &yRef,
                            const int &xCenter, const int &yCenter) override;

        virtual void renderMainPj(SDL2pp::Renderer &renderer,
                            const int &xCenter, const int &yCenter);

        void showHud(SDL2pp::Renderer &renderer, const int &windowWidth, const int &windowHeight);
        
        void updateHealth(const int &newHealthPoint);

        void updateScore(const int &newScore);

        void updateState(const std::string &newState);

        void updateDirection(bool &isFacingRight);
};

#endif
