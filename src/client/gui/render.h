#ifndef RENDER_H
#define RENDER_H

#include <SDL2pp/SDL2pp.hh>

#include "estado_juego.h"

using namespace SDL2pp;

class Render {
   public:
    explicit Render(Window& window, const int &id);

    void presentGame2(UpdatableGameState2 gameState, MapInfo mapInfo);
    
    void copyMap(MapInfo mapInfo);

    void presentImage();

    void sleep(int millisecond);

    //void presentGame(GameStateRenderer, MapInfo mapInfo);
    //void copyPlayer(PlayerState jugador);

   private:
    Window& window;
    Renderer renderer;
    Texture mapsTexture;

      const int mainPlayerID;
    const int xCenter;
    const int yCenter;
    int xReference;
    int yReference;

    void copyMapPart(int typeOfPart, int part, std::vector<Position> positions);

    void copyWall();

    void copyEntity(int posX, int posY, int spriteLong, int spriteHigh,
                    int animationLong, Texture& sprite);
};

#endif