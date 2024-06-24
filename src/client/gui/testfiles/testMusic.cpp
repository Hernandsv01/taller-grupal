#include "../soundManager.h"

int main() {
    SDL2pp::SDL sdl(SDL_INIT_AUDIO);

    SoundManager::Init();
    SharedMusicPtr music = SoundManager::getSound("diamondMap");
    SoundManager::playMusic(music);
}