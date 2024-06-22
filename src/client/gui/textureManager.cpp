#include "textureManager.h"

#include <filesystem>
#include <iostream>
#include <vector>

#ifndef DATA_PATH
#define DATA_PATH "src/client/gui/data/"
#endif

#define BACKGROUND_TEXTURES_PATH "src/textures/backgrounds/"
#define TILE_TEXTURES_PATH "src/textures/tiles/"

#define JAZZSTAND "/jazz_stand.png"
#define JAZZRUN "/jazz_run.png"
#define JAZZDASH "/jazz_dash.png"
#define JAZZINTOX "/jazz_intox.png"
#define JAZZINTOXWALK "/jazz_intoxwalk.png"

#define JAZZFALL "/jazz_fall.png"
#define JAZZJUMP "/jazz_jump.png"
#define JAZZSHOT "/jazz_shot.png"
#define JAZZSHOTFALL "/jazz_shotfall.png"

#define EXTENSION ".png"
const std::vector<std::string> typeOfCharacter = {"Jazz"};
const std::vector<std::string> typeOfState = {"Stand", "Run",       "Dash",
                                              "Intox", "Intoxwalk", "Fall",
                                              "Jump",  "Shot",      "Shotfall"};
// "Gethit", "Roasted",
// "Special", "Hud"};

// Defino el mapa estatico
std::map<std::string, std::shared_ptr<SDL2pp::Texture>>
    TextureManager::textures;

std::map<std::string, SDL2pp::Texture*> TextureManager::backgrounds;

std::map<std::string, SDL2pp::Texture*> TextureManager::tiles;

void TextureManager::load_textures_from_path_into_map(
    SDL2pp::Renderer& renderer, const std::string& path,
    std::map<std::string, SDL2pp::Texture*>& map) {
    for (const auto& texture_path :
         std::filesystem::recursive_directory_iterator(path)) {
        // This is the filename without extension.
        std::string texture_id = texture_path.path().stem().string();
        SDL2pp::Texture* texture =
            new SDL2pp::Texture(renderer, texture_path.path().string());

        std::cout << "Created texture: " << texture_path.path().string()
                  << " with id: " << texture_id << std::endl;
        map[texture_id] = texture;
    }
}

void TextureManager::Init(SDL2pp::Renderer& renderer) {
    for (std::string character : typeOfCharacter) {
        for (std::string state : typeOfState) {
            std::string textureName = character + state;
            SDL2pp::Texture texture(renderer,
                                    DATA_PATH + textureName + EXTENSION);
            textures[textureName] =
                std::make_shared<SDL2pp::Texture>(std::move(texture));
        }
    }

    load_textures_from_path_into_map(renderer, BACKGROUND_TEXTURES_PATH,
                                     backgrounds);
    load_textures_from_path_into_map(renderer, TILE_TEXTURES_PATH, tiles);

    /*
    SDL2pp::Texture jazzStand(renderer, DATA_PATH JAZZSTAND);
    textures["JazzStand"] =
    std::make_shared<SDL2pp::Texture>(std::move(jazzStand));

    SDL2pp::Texture jazzRun(renderer, DATA_PATH JAZZRUN);
    textures["JazzRun"] = std::make_shared<SDL2pp::Texture>(std::move(jazzRun));

    SDL2pp::Texture jazzIntox(renderer, DATA_PATH JAZZINTOX);
    textures["JazzIntox"] =
    std::make_shared<SDL2pp::Texture>(std::move(jazzIntox));

    SDL2pp::Texture jazzIntoxWalk(renderer, DATA_PATH JAZZINTOXWALK);
    textures["JazzIntoxWalk"] =
    std::make_shared<SDL2pp::Texture>(std::move(jazzIntoxWalk));

    SDL2pp::Texture jazzDash(renderer, DATA_PATH JAZZDASH);
    textures["JazzDash"] =
    std::make_shared<SDL2pp::Texture>(std::move(jazzDash));
    */

    /*
    SDL2pp::Texture jazzIntox(renderer, DATA_PATH JAZZINTOX);
    textures["JazzIntox"] =
    std::make_shared<SDL2pp::Texture>(std::move(jazzIntox)); SDL2pp::Texture
    jazzIntoxWalk(renderer, DATA_PATH JAZZINTOXWALK); textures["JazzIntoxWalk"]
    = std::make_shared<SDL2pp::Texture>(std::move(jazzIntoxWalk));
    SDL2pp::Texture jazzGetHit(renderer, DATA_PATH JAZZGETHIT);
    textures["JazzGetHit"] =
    std::make_shared<SDL2pp::Texture>(std::move(jazzGetHit));
    */
}

std::shared_ptr<SDL2pp::Texture> TextureManager::getTexture(
    const std::string& textureName) {
    try {
        return textures.at(textureName);
    } catch (const std::out_of_range& e) {
        std::cerr << "ACA ESTAB EL ERROR: " << e.what() << std::endl;
        return nullptr;
    }
}

SDL2pp::Texture& TextureManager::getBackground(const std::string& texture_id) {
    return *(TextureManager::backgrounds.at(texture_id));
}

SDL2pp::Texture& TextureManager::getTile(const std::string& texture_id) {
    return *(TextureManager::tiles.at(texture_id));
}

TextureManager::~TextureManager() {
    for (auto& texture : backgrounds) {
        delete texture.second;
    }
    for (auto& texture : tiles) {
        delete texture.second;
    }
}