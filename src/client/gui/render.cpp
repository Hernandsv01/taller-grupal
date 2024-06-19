#include "render.h"

#define BASESPRITE 0
#define JAZZSTANDX 46
#define JAZZSTANDY 49
#define STANDSPRITELONG 27

#define JAZZRUNX 67
#define JAZZRUNY 35
#define RUNSPRITELONG 8

#define JAZZINTOXX 42
#define JAZZINTOXY 48
#define INTOXSPRITELONG 8

#define JAZZINTOXWALKX 63
#define JAZZINTOXWALKY 50
#define INTOXWALKSPRITELONG 12

#define JAZZX 90
#define JAZZY 71

#define GROUND 0
#define UNDER 1
#define BACK 2
#define WALL 3
#define PARTDIMX 32
#define PARTDIMY 64
#define WALLDIMENSION 256
// Jazz stand dimension (46, 49) pix
// Jazz run dimension (67, 34) pix

#define WINDOW_TITLE "SDL2pp demo"

#ifndef DATA_PATH
#define DATA_PATH "src/client/gui/data"
#endif

// Render::Render(int width, int height)
//     : sdl(SDL_INIT_VIDEO),
//       window(Window(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED,
//                     SDL_WINDOWPOS_UNDEFINED, width, height,
//                     SDL_WINDOW_RESIZABLE)),
//       renderer(window, -1, SDL_RENDERER_ACCELERATED),
//       standSpritesJazz(renderer, DATA_PATH "/Jazz_stand.png"),
//       runSpritesJazz(renderer, DATA_PATH "/Jazz_run.png"),
//       frame(0) {}

Render::Render(Window& window)
    : window(window),
      renderer(window, -1, SDL_RENDERER_ACCELERATED),
      mapsTexture(renderer, DATA_PATH "/map_diamond.png"),
      standSpritesJazz(renderer, DATA_PATH "/Jazz_stand.png"),
      runSpritesJazz(renderer, DATA_PATH "/Jazz_run.png"),
      intoxJazz(renderer, DATA_PATH "/Jazz_intoxStand.png"),
      intoxWalkJazz(renderer, DATA_PATH "/Jazz_intoxWalk.png"),

      xCenter(window.GetWidth() / 2),
      yCenter(window.GetHeight() / 2),
      xReference(xCenter),
      yReference(yCenter),
      frame(0) {
    window.SetTitle(WINDOW_TITLE);
    window.SetSize(800, 600);
    window.SetPosition(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

void Render::presentGame(GameStateRenderer gameStatus, MapInfo mapInfo) {
    xReference = gameStatus.mainPlayer.position.x;
    yReference = gameStatus.mainPlayer.position.y;
    copyMap(mapInfo);
    copyPlayer(gameStatus.mainPlayer);
    renderer.Present();
}

void Render::copyPlayer(PlayerState jugador) {
    int xPos = jugador.position.x;
    int yPos = jugador.position.y;

    if (jugador.state == enums_value_update::Idle) {
        copyEntity(xPos, yPos, JAZZSTANDX, JAZZSTANDY, STANDSPRITELONG,
                   standSpritesJazz);
    } else if (jugador.state == enums_value_update::Running) {
        copyEntity(xPos, yPos, JAZZRUNX, JAZZRUNY, RUNSPRITELONG,
                   runSpritesJazz);
    } else if (jugador.state == enums_value_update::Intoxicated) {
        copyEntity(xPos, yPos, JAZZINTOXX, JAZZINTOXY, INTOXSPRITELONG,
                   intoxJazz);
    } else {
        copyEntity(xPos, yPos, JAZZINTOXWALKX, JAZZINTOXWALKY,
                   INTOXWALKSPRITELONG, intoxWalkJazz);
    }
}

void Render::copyEntity(int xPos, int yPos, int spriteLong, int spriteHigh,
                        int animationLong, Texture& sprite) {
    int srcX = frame * spriteLong;

    renderer.Copy(
        sprite, Rect(srcX, BASESPRITE, spriteLong, spriteHigh),
        Rect(xCenter - spriteLong / 2, yCenter, spriteLong, spriteHigh));

    frame = (frame + 1) % animationLong;
}

void Render::copyMap(MapInfo mapInfo) {
    copyWall();
    copyMapPart(GROUND, mapInfo.typeOfGround, mapInfo.groundPosition);
    copyMapPart(UNDER, mapInfo.typeOfUnder, mapInfo.underPosition);
}

void Render::copyWall() {
    for (int i = 0; i < window.GetWidth(); i += WALLDIMENSION) {
        for (int j = 0; j < window.GetHeight(); j += WALLDIMENSION) {
            renderer.Copy(
                mapsTexture,
                Rect(0, WALL * PARTDIMY, WALLDIMENSION, WALLDIMENSION),
                Rect(i, j, WALLDIMENSION, WALLDIMENSION));
        }
    }
}

void Render::copyMapPart(int typeOfPart, int part,
                         std::vector<Position> positions) {
    for (auto position : positions) {
        renderer.Copy(
            mapsTexture,
            Rect(part * PARTDIMX, typeOfPart * PARTDIMY, PARTDIMX, PARTDIMY),
            Rect(position.x - xReference + xCenter,
                 position.y - yReference + yCenter, PARTDIMX, PARTDIMY));
    }
}

void Render::presentImage() { renderer.Present(); }

void Render::sleep(int millisecond) { SDL_Delay(millisecond); }
