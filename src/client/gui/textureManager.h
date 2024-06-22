#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <map>
#include <memory>
#include <string>
#include <SDL2pp/SDL2pp.hh>

class TextureManager {
    private:
        static std::map<std::string, std::shared_ptr<SDL2pp::Texture>> textures;

    public:
        static void Init(SDL2pp::Renderer& renderer);

        static std::shared_ptr<SDL2pp::Texture> getTexture(const std::string& textureName);

        static std::shared_ptr<SDL2pp::Texture> getJazzStand();
};

#endif
