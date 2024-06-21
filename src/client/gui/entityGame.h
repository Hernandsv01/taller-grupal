#ifndef ENTITY_H
#define ENTITY_H

#include <SDL2pp/SDL2pp.hh>

class Entity2 {
    protected:
        int positionX;
        int positionY;
        bool direction;
        
    public:
        Entity2();

        virtual void updatePosition(const int &posX, const int &posY);

        virtual int getPosX() const;

        virtual int getPosY() const;

        virtual void renderize(SDL2pp::Renderer &renderer,
                            const int &xRef, const int &yRef,
                            const int &xCenter, const int &yCenter) {};

        virtual void renderMainPj(SDL2pp::Renderer &renderer,
                            const int &xCenter, const int &yCenter) {};

        virtual void updateState(const std::string &newState) {};

        virtual void updateDirection(bool &isFacingRight) {};
};

#endif
