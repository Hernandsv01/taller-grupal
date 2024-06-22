#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <SDL2pp/SDL2pp.hh>
#include <map>
#include <memory>
#include <string>

class TextureManager {
   private:
    static std::map<std::string, std::shared_ptr<SDL2pp::Texture>> textures;
    static std::map<std::string, SDL2pp::Texture*> backgrounds;
    static std::map<std::string, SDL2pp::Texture*> tiles;

    static void load_textures_from_path_into_map(
        SDL2pp::Renderer& renderer, const std::string& path,
        std::map<std::string, SDL2pp::Texture*>& map);

   public:
    static void Init(SDL2pp::Renderer& renderer);

    static std::shared_ptr<SDL2pp::Texture> getTexture(
        const std::string& textureName);

    static SDL2pp::Texture& getBackground(const std::string& texture_id);
    static SDL2pp::Texture& getTile(const std::string& texture_id);

    static std::shared_ptr<SDL2pp::Texture> getJazzStand();

    ~TextureManager();
};

#endif
