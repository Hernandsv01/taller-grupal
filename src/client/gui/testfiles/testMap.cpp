#include <SDL2pp/SDL2pp.hh>

#include "../estado_juego.h"
#include "../render.h"

// Map dimensions (768, 1024)
// Terrain parts (32, 64)
// Background (256, 256)

int main() {
    int width = 640;
    int height = 480;

    SDL sdl(SDL_INIT_VIDEO);
    Window window("SDL2pp demo", SDL_WINDOWPOS_UNDEFINED,
                  SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_RESIZABLE);
    Render render(window);

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

    while (1) {
        render.copyMap(mapInfo);
        render.presentImage();
        // Simulate 30 fps, 33.333 milliseconds
        render.sleep(50);
    }

    // Here all resources are automatically released and library deinitialized
    return 0;
}
