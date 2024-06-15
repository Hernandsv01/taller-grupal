#include <SDL2pp/SDL2pp.hh>

#include "../estado_juego.h"
#include "../render.h"

#define XPICPIX 134
#define RUNSPEED 10

int main() {
    int width = 640;
    int height = 480;

    // Map
    std::vector<Position> positionGround;
    for (int i = 0; i < 640; i += 32) {
        positionGround.push_back(Position{i, 300});
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

    // Initialize SDL structures
    SDL sdl(SDL_INIT_VIDEO);
    Window window("SDL2pp demo", SDL_WINDOWPOS_UNDEFINED,
                  SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_RESIZABLE);
    Render render(window);

    // Player position
    int pjPosX = 640 / 2;
    int pjPosY = 300;
    int cantMaxFrames = 50;

    for (int i = 0; i < cantMaxFrames; i++) {
        PlayerState personajeJazz;
        personajeJazz.position = Position{pjPosX, pjPosY};
        personajeJazz.direction = Direction::Right;
        personajeJazz.characterType = Jazz;
        personajeJazz.healthPoints = 10;
        personajeJazz.state.state = Intoxicated;
        personajeJazz.score = 0;

        // Game status
        GameStateRenderer gameCondition;
        gameCondition.mainPlayer = personajeJazz;

        render.presentGame(gameCondition, mapInfo);
        // Simulate 30 fps, 33.333 milliseconds
        render.sleep(33);
    }

    while (pjPosX <= width) {
        // Main player

        PlayerState personajeJazz;
        personajeJazz.position = Position{pjPosX, pjPosY};
        personajeJazz.direction = Direction::Right;
        personajeJazz.characterType = Jazz;
        personajeJazz.healthPoints = 10;
        personajeJazz.state.state = IntoxWalking;
        personajeJazz.score = 0;

        // Game status
        GameStateRenderer gameCondition;
        gameCondition.mainPlayer = personajeJazz;

        render.presentGame(gameCondition, mapInfo);
        pjPosX += 10;
        // Simulate 30 fps, 33.333 milliseconds
        render.sleep(33);
    }

    // Here all resources are automatically released and library deinitialized
    return 0;
}