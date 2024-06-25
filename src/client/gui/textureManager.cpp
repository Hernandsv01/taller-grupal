#include "textureManager.h"

#include <filesystem>
#include <iostream>
#include <vector>

#ifndef DATA_PATH
#define DATA_PATH "src/client/gui/data/"
#endif

#define WEAPON "Weapon"

#define BACKGROUND_TEXTURES_PATH "src/textures/backgrounds/"
#define TILE_TEXTURES_PATH "src/textures/tiles/"
#define ENTITY_TEXTURE_PATH "src/textures/entities"

#define EXTENSION ".png"
#define INFTEXTURE "infinity"

const std::vector<std::string> typeOfCharacter = {"Jazz"};
const std::vector<std::string> typeOfState = {
    "Stand", "Run",      "Dash",   "Intox", "Intoxwalk", "Fall",   "Jump",
    "Shot",  "Shotfall", "Gethit", "Hud",   "Dead",      "Special"};

const std::vector<std::string> typeOfWeapons = {"Default", "Rapid"};
// "Gethit", "Roasted",
// "Special", "Hud"};

// Defino el mapa estatico (nombre: textura)
std::map<std::string, std::shared_ptr<SDL2pp::Texture>>
    TextureManager::textures;

std::map<std::string, SDL2pp::Texture*> TextureManager::backgrounds;
std::map<std::string, SDL2pp::Texture*> TextureManager::tiles;
std::map<std::string, SDL2pp::Texture*> TextureManager::entity_textures;

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
    // Carga numeros
    for (int i = 0; i < 10; i++) {
        std::string textureName = std::to_string(i);
        SDL2pp::Texture texture(renderer, DATA_PATH + textureName + EXTENSION);
        textures[textureName] =
            std::make_shared<SDL2pp::Texture>(std::move(texture));
    }
    // Carga armas
    for (auto weapon : typeOfWeapons) {
        std::string textureName = WEAPON + weapon;
        SDL2pp::Texture texture(renderer, DATA_PATH + textureName + EXTENSION);
        textures[textureName] =
            std::make_shared<SDL2pp::Texture>(std::move(texture));
    }
    std::string textureName = INFTEXTURE;
    SDL2pp::Texture texture(renderer, DATA_PATH + textureName + EXTENSION);
    textures[textureName] =
        std::make_shared<SDL2pp::Texture>(std::move(texture));

    std::string textureName2 = "separator";
    SDL2pp::Texture texture2(renderer, DATA_PATH + textureName2 + EXTENSION);
    textures[textureName2] =
        std::make_shared<SDL2pp::Texture>(std::move(texture2));

    load_textures_from_path_into_map(renderer, BACKGROUND_TEXTURES_PATH,
                                     backgrounds);
    load_textures_from_path_into_map(renderer, TILE_TEXTURES_PATH, tiles);

    load_textures_from_path_into_map(renderer, ENTITY_TEXTURE_PATH,
                                     entity_textures);
}

std::shared_ptr<SDL2pp::Texture> TextureManager::getTexture(
    const std::string& textureName) {
    try {
        // std::cout << "En el manager: " << textureName << "\n";
        return textures.at(textureName);
    } catch (const std::out_of_range& e) {
        std::cerr << "ACA ESTAB EL ERROR: " << e.what()
                  << " Textura:" << textureName << std::endl;
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
    for (auto& texture : entity_textures) {
        delete texture.second;
    }
}
SDL2pp::Texture* TextureManager::getEntityTexture(
    const std::string& textureName) {
    try {
        return entity_textures.at(textureName);
    } catch (const std::out_of_range& e) {
        return entity_textures["placeholder"];
    }
}
