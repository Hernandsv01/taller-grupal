#include "../soundManager.h"

//old version 

// int main() {
//     SDL2pp::SDL sdl(SDL_INIT_AUDIO);

//     SoundManager::Init();
//     SharedMusicPtr music = SoundManager::getSound("prueba");
//     SoundManager::playMusic(music);
// }


//new version
/**/
int main(int argc, char* argv[]) {
    SoundManager::Init();
    Mix_Chunk* music = SoundManager::LoadMusic("/home/lara/Desktop/Taller/taller-grupal/src/client/gui/testfiles/prueba.mp3");
    if (!music ) {
        SoundManager::Cleanup();
        return 1;
    }

    SoundManager::SetMusicVolume(0, 20); //20% 

    SoundManager::PlayMusic(music, 0, -1); 
    Mix_Chunk* music2 = SoundManager::LoadMusic("/home/lara/Desktop/Taller/taller-grupal/src/client/gui/data/pop.mp3");

    if (!music2 ) {
        SoundManager::Cleanup();
        return 1;
    }
    SoundManager::PlayMusic(music2, 1, 0); 
    SoundManager::SetMusicVolume(1, 50); //50% 
    std::cout << "Press Enter to stop the music..." << std::endl;
    std::cin.get();
    SoundManager::StopMusic(0);
    Mix_FreeChunk(music); 
    
    SoundManager::StopMusic(1);
    Mix_FreeChunk(music2);
    Mix_CloseAudio();
    SDL_Quit();

    return 0;
}





