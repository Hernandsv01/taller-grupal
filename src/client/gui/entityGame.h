#ifndef ENTITY_H
#define ENTITY_H

#include <SDL2pp/SDL2pp.hh>

class Entity2 {
   protected:
    int positionX = 0;
    int positionY = 0;
    bool isRight = true;
    int actualSpriteNumber = 0;
    bool loopAnimation = true;

    std::string texture = "";

   public:
    Entity2();

    Entity2(std::string texture);

    virtual void updatePosition(const int &posX, const int &posY);

    virtual int getPosX() const;

    virtual int getPosY() const;

    virtual void renderize(SDL2pp::Renderer &renderer, const int &xRef,
                           const int &yRef, uint32_t tick);

    void renderizeWithTexture(SDL2pp::Renderer &renderer,
                              SDL2pp::Texture &texture, const int &xRef,
                              const int &yRef, uint32_t tick);

    virtual void updateState(const std::string &newState) {};

    virtual void updateHealth(const int &newHealth) {};

    virtual void updateWeapon(const std::string &newWeapon) {};

    virtual void updateAmmoQuantity(const int &newAmmoQuantity) {};

    virtual void updateDirection(bool &isFacingRight) {
        isRight = isFacingRight;
    };

    virtual bool isPlayer();
};

#endif
