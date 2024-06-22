#include "render.h"

#include "textureManager.h"

#define BASESPRITE 0

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

Render::Render(Window& window, const int& id)
    : window(window),
      renderer(window, -1, SDL_RENDERER_ACCELERATED),
      mapsTexture(renderer, DATA_PATH "/map_diamond.png"),
      mainPlayerID(id),
      xCenter(window.GetWidth() / 2),
      yCenter(window.GetHeight() / 2),
      xReference(0),
      yReference(0) {
    TextureManager::Init(renderer);
}

void Render::presentGame2(UpdatableGameState2 gameState, Map& map) {
    xCenter = window.GetWidth() / 2;
    yCenter = window.GetHeight() / 2;

    xReference = gameState.getEntityPositionX(mainPlayerID);
    yReference = gameState.getEntityPositionY(mainPlayerID);

    // Renderizar mapa
    renderMap(map);

    // Renderiza ¿entidades? ¿solo jugadores?
    gameState.copyAllEntities(this->renderer, mainPlayerID, xCenter, yCenter);

    // Renderiza UI

    renderer.Present();
}

void Render::renderMap(Map& map) {
    renderMapBackground(map);

    std::vector<BlockOnlyTexture> map_tiles = map.get_all_block_textures();

    for (const BlockOnlyTexture& block : map_tiles) {
        renderer.Copy(
            TextureManager::getTile(block.texture), NullOpt,
            Rect((block.coordinate.x) * sizeFactor - xReference + xCenter,
                 (block.coordinate.y) * sizeFactor - yReference + yCenter,
                 sizeFactor, sizeFactor));
    }
}

void Render::renderMapBackground(Map& map) {
    IdTexture background_id = map.get_background();

    SDL2pp::Texture& background_texture =
        TextureManager::getBackground(background_id);

    auto texture_size = background_texture.GetSize();

    for (int x = 0; x < window.GetWidth(); x += texture_size.x) {
        for (int y = 0; y < window.GetHeight(); y += texture_size.y) {
            auto start_point = Point(x, y);
            auto rect = Rect(start_point, texture_size);

            renderer.Copy(background_texture, NullOpt, rect);
        }
    }
}

void Render::presentImage() { renderer.Present(); }

void Render::sleep(int millisecond) { SDL_Delay(millisecond); }
