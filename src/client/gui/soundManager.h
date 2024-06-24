#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <string>
#include <map>
#include <memory>
#include <SDL2pp/SDL2pp.hh>
#include <SDL2pp/Music.hh>
#include <SDL2pp/Mixer.hh>

typedef std::shared_ptr<SDL2pp::Music> SharedMusicPtr;

class SoundManager {
    private:
        static std::map<std::string, SharedMusicPtr> sounds;

    public:
        static SDL2pp::Mixer* mixer;

        static void Init();
        static SharedMusicPtr getSound(const std::string& soundName);
        static void playMusic(SharedMusicPtr);
};

#endif
