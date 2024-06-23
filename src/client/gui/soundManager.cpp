#include <iostream>
#include "soundManager.h"

#define CHUNKSIZE 4096

SDL2pp::Mixer* SoundManager::mixer = nullptr;

void SoundManager::Init() {
    mixer = new SDL2pp::Mixer(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, CHUNKSIZE);
}

SoundManager::SharedMusicPtr SoundManager::getSound(const std::string& soundName) {
    std::string path = "src/client/gui/data/" + soundName + ".wav";
    std::cout << "El archivo es: " << path << "\n";
    return std::make_shared<SDL2pp::Music>(path);
}