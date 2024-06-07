#include <SDL2pp/SDL2pp.hh>

#include "../estado_juego.h"
#include "../render.h"

int main() {
    int width = 640;
    int height = 480;
    int pjPosX = width / 2;
    int pjPosY = 300;
    int groundPosY = 300;

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

    // Jugador principal
    std::vector<State> playerState = {Idle};

    PlayerState mainPlayer;
    mainPlayer.characterType = Jazz;
    mainPlayer.healthPoints = 10;
    mainPlayer.states = playerState;
    mainPlayer.score = 0;
    mainPlayer.position = Position{pjPosX, pjPosY};
    mainPlayer.direction = Direction::Right;

    // State del juego
    GameStateRenderer gameCondition;
    gameCondition.mainPlayer = mainPlayer;

    // Initialize SDL structures
    SDL sdl(SDL_INIT_VIDEO);
    Window window("SDL2pp demo", SDL_WINDOWPOS_UNDEFINED,
                  SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_RESIZABLE);
    Render render(window);

    while (1) {
        render.presentGame(gameCondition, mapInfo);

        // Simulate 30 fps, 33.333 milliseconds
        render.sleep(34);
    }

    // Here all resources are automatically released and library deinitialized
    return 0;
}