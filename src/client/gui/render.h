#ifndef RENDER_H
#define RENDER_H

#include <SDL2pp/SDL2pp.hh>

#include "../../common/map/map.h"
#include "estado_juego.h"

using namespace SDL2pp;

class Render {
   public:
    explicit Render(Window& window, const int& id);

    void presentGame2(UpdatableGameState2 gameState, Map& map);

    // void presentGame(GameStateRenderer, MapInfo mapInfo);
    // void copyPlayer(PlayerState jugador);

   private:
    Window& window;
    Renderer renderer;
    Texture mapsTexture;

    int sizeFactor = FACTOR_TAMANIO;

    const int mainPlayerID;
    int xCenter;
    int yCenter;
    int xReference;
    int yReference;

    void copyEntity(int posX, int posY, int spriteLong, int spriteHigh,
                    int animationLong, Texture& sprite);

    void renderMap(Map& map);

    void renderMapBackground(Map& map);
};

#endif