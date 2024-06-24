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
    Mix_Music* music = SoundManager::LoadMusic("/home/lara/Desktop/Taller/taller-grupal/src/client/gui/testfiles/prueba.mp3");
    Mix_Music* music2 = SoundManager::LoadMusic("/home/lara/Desktop/Taller/taller-grupal/src/client/gui/testfiles/prueba.mp3");
    if (!music || music2) {
        SoundManager::Cleanup();
        return 1;
    }

    SoundManager::SetMusicVolume(20); //20% 

    SoundManager::PlayMusic(music, -1); 
    SoundManager::PlayMusic(music2, 1); 
    std::cout << "Press Enter to stop the music..." << std::endl;
    std::cin.get();
    SoundManager::StopMusic();
    Mix_FreeMusic(music);
    SoundManager::Cleanup();

    return 0;
}





