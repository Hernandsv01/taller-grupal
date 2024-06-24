#include <iostream>
#include "soundManager.h"

#define CHUNKSIZE 4096
#define SOUNDPATH "taller-grupal/src/client/gui/testfiles/"
#define EXTENSION ".mp3"

#define SOUNDNAME "prueba"



std::map<std::string, Mix_Music*> SoundManager::sounds;

void SoundManager::Init() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        return;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
        return;
    }
}

void SoundManager::Cleanup() {
    for (auto& pair : sounds) {
        Mix_FreeMusic(pair.second);
    }
    sounds.clear();

    Mix_CloseAudio();
    SDL_Quit();
}

Mix_Music* SoundManager::LoadMusic(const std::string& soundFilePath) {
    Mix_Music* music = Mix_LoadMUS(soundFilePath.c_str());
    return music;
}

void SoundManager::PlayMusic(Mix_Music* music, int loops) {
    if (Mix_PlayMusic(music, loops) == -1) {
        std::cerr << "Failed to play music! SDL_mixer Error: " << Mix_GetError() << std::endl;
    }
}

void SoundManager::StopMusic() {
    Mix_HaltMusic();
}

void SoundManager::SetMusicVolume(int volume) {
    Mix_VolumeMusic(volume); 
}



//OLD VERSION
// std::map<std::string, SharedMusicPtr> SoundManager::sounds;
// SDL2pp::Mixer* SoundManager::mixer = nullptr;

// void SoundManager::Init() {
//     mixer = new SDL2pp::Mixer(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, CHUNKSIZE);
//     SDL2pp::Music music(std::string(SOUNDPATH) + SOUNDNAME + EXTENSION);
//     sounds[SOUNDNAME] = std::make_shared<SDL2pp::Music>(std::move(music));
// }

// SharedMusicPtr SoundManager::getSound(const std::string& soundName) {
//     return sounds[soundName];
// }

// void SoundManager::playMusic(SharedMusicPtr music) {
//     mixer->PlayMusic(*music, -1);
// }