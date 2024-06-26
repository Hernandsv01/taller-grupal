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
    std::string state = "";
    int secondId = 2;
    bool secondIsLeft = false;

    // Initialize SDL structures
    SDL sdl(SDL_INIT_VIDEO);
    Window window("SDL2pp demo", SDL_WINDOWPOS_UNDEFINED,
                  SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_RESIZABLE);
    
    Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);
    TextureManager::Init(renderer);

    // Map
    std::vector<Position> positionGround;
    for (int i = 0; i < 640; i += 32) {
        positionGround.push_back(Position{i, groundPosY});
    }
    std::vector<Position> positionUnder;
    for (int i = 0; i < 640; i += 32) {
        positionUnder.push_back(Position{i, 364});
    }
    MapInfo mapInfo;
    mapInfo.mapTexture = Diamond;
    mapInfo.typeOfGround = RightSign;
    mapInfo.groundPosition = positionGround;
    mapInfo.typeOfUnder = RedDiamond;
    mapInfo.underPosition = positionUnder;

    // State del juego, main player
    UpdatableGameState2 gameState;
    gameState.addEntity(mainPlayerID, playerType, jazzType);
    gameState.updatePosition(mainPlayerID, pjPosX, pjPosY);
    gameState.updateState(mainPlayerID, state);

    //Second player
    gameState.addEntity(secondId, playerType, jazzType);
    gameState.updatePosition(secondId, pjPosX-200, pjPosY);
    gameState.updateState(secondId, state);
    gameState.updateDirection(secondId, secondIsLeft);
    
    while (1) {

        gameState.copyAllEntities(renderer, mainPlayerID, pjPosX, pjPosY);
        renderer.Present();        

        // Simulate 30 fps, 33.333 milliseconds
        SDL_Delay(200);
    }

    // Here all resources are automatically released and library deinitialized
    return 0;
}
