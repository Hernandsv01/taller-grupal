#ifndef RENDER_H
#define RENDER_H

#include <SDL2pp/SDL2pp.hh>

#include "estado_juego.h"

using namespace SDL2pp;

class Render {
    
    public:
        explicit Render(Window& window);

        void presentGame(EstadoJuegoRenderer, MapInfo mapInfo);

        void copyPlayer(EstadoJugador jugador);
        void copyMap(MapInfo mapInfo);

        void presentImage();

        void sleep(int millisecond);

    private:
        Window& window;
        Renderer renderer;

        Texture mapsTexture;
        Texture standSpritesJazz;
        Texture runSpritesJazz;
        Texture intoxJazz;
        Texture intoxWalkJazz;
        

        int xCenter;
        int yCenter;
        int xReference;
        int yReference;
        int frame;
    
        void copyMapPart(int typeOfPart ,int part, std::vector<Posicion> positions);
        
        void copyWall();

        void copyEntity(int posX, int posY, int spriteLong, int spriteHigh,
                    int animationLong, Texture& sprite);
};

#endif