#include "../estado_juego.h"
#include "../render.h"

#define XPICPIX 134
#define RUNSPEED 10
// Jazz run dimension (134, 34) pix

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

    SDL sdl(SDL_INIT_VIDEO);
    Window window("SDL2pp demo", SDL_WINDOWPOS_UNDEFINED,
                  SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_RESIZABLE);
    Render render(window);
    int pjPosX = 0;
    int pjPosY = 300;

    while (pjPosX <= width) {
        // Main player
        std::vector<State> personajeEstados = {Running};

        PlayerState personajeJazz;
        personajeJazz.position = Position{pjPosX, pjPosY};
        personajeJazz.direction = Direction::Right;
        personajeJazz.characterType = Jazz;
        personajeJazz.healthPoints = 10;
        personajeJazz.states = personajeEstados;
        personajeJazz.score = 0;

        // Game status
        GameStateRenderer gameCondition;
        gameCondition.mainPlayer = personajeJazz;

        render.presentGame(gameCondition, mapInfo);
        // int xPosBefore = personajeJazz.position.x;
        // int yPosBefore = personajeJazz.position.y;
        // personajeJazz.position = Position{xPosBefore+10, yPosBefore};
        // Simulate 30 fps, 33.333 milliseconds
        pjPosX += 10;
        render.sleep(33);
    }

    // Here all resources are automatically released and library deinitialized
    return 0;
}