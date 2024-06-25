
#include "soundManager.h"

#include <filesystem>
#include <iostream>

#define CHUNKSIZE 4096

#ifndef SOUND_PATH
#define SOUND_PATH ""
#endif

std::map<std::string, Mix_Chunk*> SoundManager::sounds;

void SoundManager::Init() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
        return;
    }

    SetMusicVolume(MUSIC_CHANNEL, 10);
    SetMusicVolume(SHOOT_CHANNEL, 20);
    SetMusicVolume(HURT_CHANNEL, 10);

    LoadMusic();
}

SoundManager::~SoundManager() {
    Mix_CloseAudio();

    for (auto& sound : sounds) {
        Mix_FreeChunk(sound.second);
    }
}

void SoundManager::LoadMusic() {
    for (const auto& sound_path :
         std::filesystem::recursive_directory_iterator(SOUND_PATH)) {
        // This is the filename without extension.
        std::string sound_id = sound_path.path().stem().string();

        Mix_Chunk* music = Mix_LoadWAV(sound_path.path().string().c_str());
        sounds.insert(std::make_pair(sound_id, music));
    }
}

void SoundManager::PlayMusic(std::string sound_id, int channel, int loops) {
    Mix_Chunk* sound = sounds.at(sound_id);

    Mix_PlayChannel(channel, sound, loops);
}

void SoundManager::StopMusic(int channel) { Mix_HaltChannel(channel); }

void SoundManager::SetMusicVolume(int channel, int volume) {
    Mix_Volume(channel, volume);
}

// OLD VERSION
//  std::map<std::string, SharedMusicPtr> SoundManager::sounds;
//  SDL2pp::Mixer* SoundManager::mixer = nullptr;

// void SoundManager::Init() {
//     mixer = new SDL2pp::Mixer(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT,
//     2, CHUNKSIZE); SDL2pp::Music music(std::string(SOUNDPATH) + SOUNDNAME
//     + EXTENSION); sounds[SOUNDNAME] =
//     std::make_shared<SDL2pp::Music>(std::move(music));
// }

// SharedMusicPtr SoundManager::getSound(const std::string& soundName) {
//     return sounds[soundName];
// }

// void SoundManager::playMusic(SharedMusicPtr music) {
//     mixer->PlayMusic(*music, -1);
// }
