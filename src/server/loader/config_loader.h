#ifndef CONFIG_LOADER_H
#define CONFIG_LOADER_H

#include "yaml-cpp/yaml.h"



struct Config{
    GameConfig game;
    EnemyConfig enemy;
    PlayerConfig player;
    AssetsConfig assets;
}

class ConfigLoader{
private:

public: 
    static int load(std::string file_name);
    
};

#endif // CONFIG_LOADER_H