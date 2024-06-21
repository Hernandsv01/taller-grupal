#include "textureManager.h"
#include <iostream>
#include <vector>

#ifndef DATA_PATH
#define DATA_PATH "src/client/gui/data"
#endif

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
const std::vector<std::string> typeOfState = {"Stand", "Run", "Dash",
                                    "Intox", "Intoxwalk", "Fall", "Jump",
                                    "Shot", "Shotfall", "Gethit", "Roasted", 
                                    "Special", "Hud"};

//Defino el mapa estatico
std::map<std::string, std::shared_ptr<SDL2pp::Texture>> TextureManager::textures;

void TextureManager::Init(SDL2pp::Renderer& renderer) {
    for (std::string character : typeOfCharacter) {
        for (std::string state : typeOfState) {
            std::string textureName = character + state;
            SDL2pp::Texture texture(renderer, DATA_PATH+textureName+EXTENSION);
            textures[textureName] = std::make_shared<SDL2pp::Texture>(std::move(texture));
        }
        
    }
    /*
    SDL2pp::Texture jazzStand(renderer, DATA_PATH JAZZSTAND);
    textures["JazzStand"] = std::make_shared<SDL2pp::Texture>(std::move(jazzStand));
    
    SDL2pp::Texture jazzRun(renderer, DATA_PATH JAZZRUN);
    textures["JazzRun"] = std::make_shared<SDL2pp::Texture>(std::move(jazzRun));
    
    SDL2pp::Texture jazzIntox(renderer, DATA_PATH JAZZINTOX);
    textures["JazzIntox"] = std::make_shared<SDL2pp::Texture>(std::move(jazzIntox));
    
    SDL2pp::Texture jazzIntoxWalk(renderer, DATA_PATH JAZZINTOXWALK);
    textures["JazzIntoxWalk"] = std::make_shared<SDL2pp::Texture>(std::move(jazzIntoxWalk));
    
    SDL2pp::Texture jazzDash(renderer, DATA_PATH JAZZDASH);
    textures["JazzDash"] = std::make_shared<SDL2pp::Texture>(std::move(jazzDash));
    */
    
    /*
    SDL2pp::Texture jazzIntox(renderer, DATA_PATH JAZZINTOX);
    textures["JazzIntox"] = std::make_shared<SDL2pp::Texture>(std::move(jazzIntox));
    SDL2pp::Texture jazzIntoxWalk(renderer, DATA_PATH JAZZINTOXWALK);
    textures["JazzIntoxWalk"] = std::make_shared<SDL2pp::Texture>(std::move(jazzIntoxWalk));
    SDL2pp::Texture jazzGetHit(renderer, DATA_PATH JAZZGETHIT);
    textures["JazzGetHit"] = std::make_shared<SDL2pp::Texture>(std::move(jazzGetHit));
    */
}

std::shared_ptr<SDL2pp::Texture> TextureManager::getTexture(const std::string& textureName) {
    return textures.at(textureName);
}


