#include "config.h"


void Config::load_config(const std::string& file_path){
    try {

        YAML::Node config = YAML::LoadFile(file_path);
    
        //game
        if(config["game"]["time"]) Config::game.time = config["game"]["time"].as<int>();

        //pickups
        if(config["pickups"]["coin"]) Config::pickups.coin = config["pickups"]["coin"].as<int>();
        if(config["pickups"]["carrot"]) Config::pickups.carrot = config["pickups"]["carrot"].as<int>();
        if(config["pickups"]["light_ammo"]) Config::pickups.light_ammo = config["pickups"]["light_ammo"].as<int>();
        if(config["pickups"]["heavy_ammo"]) Config::pickups.heavy_ammo = config["pickups"]["heavy_ammo"].as<int>();
        if(config["pickups"]["heavy_ammo"]) Config::pickups.power_ammo = config["pickups"]["heavy_ammo"].as<int>();

        //bullets
        

        //enemies
        auto load_enemy_attributes = [](YAML::Node node, EnemyConfig::Attributes &attr) {
            if (node["count"]) attr.count = node["count"].as<int>();
            if (node["life"]) attr.life = node["life"].as<int>();
            if (node["speed"]) attr.speed = node["speed"].as<float>();
            if (node["damage"]) attr.damage = node["damage"].as<int>();
        };

        load_enemy_attributes(config["enemy"]["flyer"], Config::enemy.flyer);
        load_enemy_attributes(config["enemy"]["shooter"], Config::enemy.shooter);
        load_enemy_attributes(config["enemy"]["crawler"], Config::enemy.crawler);

        //player
        if(config["player"]["speed"]) Config::player.speed = config["player"]["speed"].as<float>();
        if(config["player"]["jump"]) Config::player.jump = config["player"]["jump"].as<int>();
        if(config["player"]["max_health"]) Config::player.max_health = config["player"]["max_health"].as<int>();

    } catch(const YAML::BadFile &e){
        std::cerr << "Failed to load config file: " << e.what() << ". Using default values." << std::endl;
    }
    
}





int Config::get_player_speed(){
    return player.speed;
}
