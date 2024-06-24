#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <string>
#include <map>
#include <memory>
#include <SDL2pp/SDL2pp.hh>
#include <SDL2pp/Music.hh>
#include <SDL2pp/Mixer.hh>
#include <iostream>


class SoundManager {
private:
    static std::map<std::string, Mix_Music*> sounds;    
public:
    static void Init();
    static void Cleanup();
    static Mix_Chunk* LoadMusic(const std::string& soundFilePath);
    static void PlayMusic(Mix_Chunk* music, int channel, int loops);
    static void StopMusic(int channel);
    static void SetMusicVolume(int channel, int volume); //0-128

};


//old version
//typedef std::shared_ptr<SDL2pp::Music> SharedMusicPtr;
// class SoundManager {
//     private:
//         static std::map<std::string, SharedMusicPtr> sounds;

//     public:
//         //Mixer se encarga de reproducir el sonido
//         static SDL2pp::Mixer* mixer;

//         static void Init();
//         static SharedMusicPtr getSound(const std::string& soundName);
//         static void playMusic(SharedMusicPtr);
// };



#endif
