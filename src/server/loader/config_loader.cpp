#include "config_loader.h"

#include "config.h"

void ConfigLoader::load(const std::string& file_path){
    try {

        YAML::Node config = YAML::LoadFile(file_path);
    
        //game
        if(config["game"]["time"]) Config::game.time= config["game"]["time"].as<uint8_t>();

        //enemies
        auto loadEnemyAttributes = [](YAML::Node node, EnemyConfig::Attributes &attr) {
            if (node["count"]) attr.count = node["count"].as<int>();
            if (node["life"]) attr.life = node["life"].as<int>();
            if (node["speed"]) attr.speed = node["speed"].as<int>();
            if (node["damage"]) attr.damage = node["damage"].as<int>();
        };

        loadEnemyAttributes(config["enemy"]["flyer"], Config::enemy.flyer);
        loadEnemyAttributes(config["enemy"]["shooter"], Config::enemy.shooter);
        loadEnemyAttributes(config["enemy"]["crawler"], Config::enemy.crawler);


        //player
        if(config["player"]["speed"]) Config::player.speed = config["player"]["speed"].as<int>();
        if(config["player"]["jump"]) Config::player.jump = config["player"]["jump"].as<int>();
        if(config["player"]["max_health"]) Config::player.max_health = config["player"]["max_health"].as<int>();

    } catch(const YAML::BadFile &e){
        std::cerr << "Failed to load config file: " << e.what() << ". Using default values." << std::endl;
    }
    
}
