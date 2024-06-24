#include "../soundManager.h"
#include <SDL2pp/SDL2pp.hh>

int main() {
    SDL2pp::SDL sdl(SDL_INIT_AUDIO);

    SoundManager::Init();
    SoundManager::SharedMusicPtr music = SoundManager::getSound("A");
    SoundManager::mixer->PlayMusic(*music);
}