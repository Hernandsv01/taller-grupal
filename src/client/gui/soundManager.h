#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <string>
#include <memory>
#include <SDL2pp/Music.hh>
#include <SDL2pp/Mixer.hh>

class SoundManager {
public:
    typedef std::shared_ptr<SDL2pp::Music> SharedMusicPtr;
    static SDL2pp::Mixer* mixer;

    static void Init();
    static SharedMusicPtr getSound(const std::string& soundName);
    
};

#endif
