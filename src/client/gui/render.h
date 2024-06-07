#ifndef RENDER_H
#define RENDER_H

#include <SDL2pp/SDL2pp.hh>

#include "estado_juego.h"

using namespace SDL2pp;

class Render {
   public:
    //   Render(int width, int height);

    explicit Render(Window& window);

    void presentPlayer(struct PlayerState);

    void sleep(int milliSecond);

   private:
    SDL sdl;
    Window& window;
    Renderer renderer;
    Texture standSpritesJazz;
    Texture runSpritesJazz;

    int frame;

    void copyEntity(int posX, int posY, int spriteLong, int spriteHigh,
                    int animationLong, Texture& sprite);
};

#endif