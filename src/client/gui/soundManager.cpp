#include <iostream>
#include "soundManager.h"

#define CHUNKSIZE 4096
#define SOUNDPATH "src/client/gui/data/"
#define EXTENSION ".mp3"

#define SOUNDNAME "diamondMap"


std::map<std::string, SharedMusicPtr> SoundManager::sounds;
SDL2pp::Mixer* SoundManager::mixer = nullptr;

void SoundManager::Init() {
    mixer = new SDL2pp::Mixer(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, CHUNKSIZE);
    SDL2pp::Music music(std::string(SOUNDPATH) + SOUNDNAME + EXTENSION);
    sounds[SOUNDNAME] = std::make_shared<SDL2pp::Music>(std::move(music));
}

SharedMusicPtr SoundManager::getSound(const std::string& soundName) {
    return sounds[soundName];
}

void SoundManager::playMusic(SharedMusicPtr music) {
    mixer->PlayMusic(*music, -1);
}
