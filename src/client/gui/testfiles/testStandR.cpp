#include <SDL2pp/SDL2pp.hh>

#include "../estado_juego.h"
#include "../render.h"
#include "../textureManager.h"

int main() {
    int width = 640;
    int height = 480;
    int groundPosY = 300;
    int mainPlayerID = 1;
    int pjPosX = width / 2;
    int pjPosY = 300;
    int playerType = 0;
    int jazzType = 0;

    // Initialize SDL structures
    SDL sdl(SDL_INIT_VIDEO);
    Window window("SDL2pp demo", SDL_WINDOWPOS_UNDEFINED,
                  SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_RESIZABLE);
    
    Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);
    TextureManager::Init(renderer);

    // State del juego
    UpdatableGameState2 gameState;
    gameState.addEntity(mainPlayerID, playerType, jazzType);
    gameState.updatePosition(mainPlayerID, pjPosX, pjPosY);
    
    while (1) {

        
        gameState.copyAllEntities(renderer, mainPlayerID, pjPosX, pjPosY);
        renderer.Present();
        

        // Simulate 30 fps, 33.333 milliseconds
        SDL_Delay(200);
    }

    // Here all resources are automatically released and library deinitialized
    return 0;
}
