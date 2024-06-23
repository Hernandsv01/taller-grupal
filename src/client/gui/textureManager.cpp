#include "textureManager.h"

#include <iostream>
#include <vector>

#ifndef DATA_PATH
#define DATA_PATH "src/client/gui/data/"
#endif

#define WEAPON "Weapon"
#define EXTENSION ".png"
#define INFTEXTURE "infinity"

const std::vector<std::string> typeOfCharacter = {"Jazz"};
const std::vector<std::string> typeOfState = {"Stand", "Run",       "Dash",
                                              "Intox", "Intoxwalk", "Fall",
                                              "Jump",  "Shot",      "Shotfall",
                                              "Gethit", "Hud"};

const std::vector<std::string> typeOfWeapons = {"Default", "Rapid"};
// "Gethit", "Roasted",
// "Special", "Hud"};


// Defino el mapa estatico (nombre: textura)
std::map<std::string, std::shared_ptr<SDL2pp::Texture>>
    TextureManager::textures;

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
    //Carga numeros
    for (int i =0; i<10; i++) {
        std::string textureName = std::to_string(i);
        SDL2pp::Texture texture(renderer,
                                    DATA_PATH + textureName + EXTENSION);
        textures[textureName] =
                std::make_shared<SDL2pp::Texture>(std::move(texture));                           
    }
    //Carga armas
    for (auto weapon : typeOfWeapons) {
        std::string textureName = WEAPON + weapon;
        SDL2pp::Texture texture(renderer,
                                    DATA_PATH + textureName + EXTENSION);
        textures[textureName] =
                std::make_shared<SDL2pp::Texture>(std::move(texture)); 
    }
    std::string textureName = INFTEXTURE;
    SDL2pp::Texture texture(renderer,
                                DATA_PATH + textureName + EXTENSION);
    textures[textureName] =
            std::make_shared<SDL2pp::Texture>(std::move(texture)); 
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
        //std::cout << "En el manager: " << textureName << "\n";
        return textures.at(textureName);
    } catch (const std::out_of_range& e) {
        std::cerr << "ACA ESTAB EL ERROR: " << e.what() << std::endl;
        return nullptr;
    }
}
