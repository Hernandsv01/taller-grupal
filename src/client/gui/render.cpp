#include "render.h"

#define BASESPRITE 0
#define JAZZSTANDX 46
#define JAZZSTANDY 49
#define STANDSPRITELONG 27

#define JAZZRUNX 67
#define JAZZRUNY 35
#define RUNSPRITELONG 8
// Jazz stand dimension (46, 49) pix
// Jazz run dimension (67, 34) pix

#define WINDOW_TITLE "SDL2pp demo"

#define DATA_PATH "src/client/gui/data"

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
    : sdl(SDL_INIT_VIDEO),
      window(window),
      renderer(window, -1, SDL_RENDERER_ACCELERATED),
      standSpritesJazz(renderer, DATA_PATH "/Jazz_stand.png"),
      runSpritesJazz(renderer, DATA_PATH "/Jazz_run.png"),
      frame(0) {
    window.SetTitle(WINDOW_TITLE);
    window.SetSize(800, 600);
    window.SetPosition(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

void Render::presentPlayer(EstadoJugador jugador) {
    renderer.Clear();
    int xPos = jugador.posicion.x;
    int yPos = jugador.posicion.y;

    if (jugador.estados.size() == 0 || jugador.estados[0] == Parado) {
        copyEntity(xPos, yPos, JAZZSTANDX, JAZZSTANDY, STANDSPRITELONG,
                   standSpritesJazz);
    } else {
        copyEntity(xPos, yPos, JAZZRUNX, JAZZRUNY, RUNSPRITELONG,
                   runSpritesJazz);
    }
    renderer.Present();
}

void Render::copyEntity(int xPos, int yPos, int spriteLong, int spriteHigh,
                        int animationLong, Texture& sprite) {
    int srcX = frame * spriteLong;

    renderer.Copy(sprite, Rect(srcX, BASESPRITE, spriteLong, spriteHigh),
                  Rect(xPos, yPos, spriteLong, spriteHigh));

    frame = (frame + 1) % animationLong;
}

void Render::sleep(int millisecond) { SDL_Delay(millisecond); }