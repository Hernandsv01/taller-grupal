#include "config.h"

GameConfig Config::game;
PickupsConfig Config::pickups;
BulletsConfig Config::bullets;
EnemyConfig Config::enemy;
PlayerConfig Config::player;  


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
        auto load_bullets_attributes = [](YAML::Node node, BulletsConfig::Attributes &attr) {
            if(node["max_ammo"]) attr.max_ammo = node["max_ammo"].as<int>();
            if(node["speed"]) attr.speed = node["speed"].as<float>();
            if(node["damage"]) attr.damage = node["damage"].as<int>();
            if(node["bpm"]) attr.bpm = node["bpm"].as<int>();
        };

        load_bullets_attributes(config["bullets"]["normal"], Config::bullets.normal);
        load_bullets_attributes(config["bullets"]["light"], Config::bullets.light);
        load_bullets_attributes(config["bullets"]["heavy"], Config::bullets.heavy);
        load_bullets_attributes(config["bullets"]["power"], Config::bullets.power);

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

//GETTERS
//Game
int Config::get_game_time(){
    return game.time;
}

//Pickups
int Config::get_pickup_coin(){
    return pickups.coin;
}

int Config::get_pickup_carrot(){
    return pickups.carrot;
}

int Config::get_pickup_light(){
    return pickups.light_ammo;
}

int Config::get_pickup_heavy(){
    return pickups.heavy_ammo;
}

int Config::get_pickup_power(){
    return pickups.power_ammo;
}

//Bullets
//normal
int Config::get_normal_max_ammo(){
    return bullets.normal.max_ammo;
}

float Config::get_normal_speed(){
    return bullets.normal.speed;
}

int Config::get_normal_damage(){
    return bullets.normal.damage;
}

int Config::get_normal_bpm(){
    return bullets.normal.bpm;
}

//light
int Config::get_light_max_ammo(){
    return bullets.light.max_ammo;
}

float Config::get_light_speed(){
    return bullets.light.speed;
}

int Config::get_light_damage(){
    return bullets.light.damage;
}

int Config::get_light_bpm(){
    return bullets.light.bpm;
}

//heavy
int Config::get_heavy_max_ammo(){
    return bullets.heavy.max_ammo;
}

float Config::get_heavy_speed(){
    return bullets.heavy.speed;
}

int Config::get_heavy_damage(){
    return bullets.heavy.damage;
}

int Config::get_heavy_bpm(){
    return bullets.heavy.bpm;
}

//power
int Config::get_power_max_ammo(){
    return bullets.power.max_ammo;
}

float Config::get_power_speed(){
    return bullets.power.speed;
}

int Config::get_power_damage(){
    return bullets.power.damage;
}

int Config::get_power_bpm(){
    return bullets.power.bpm;
}

//Enemies
//flyer
int Config::get_flyer_count(){
    return enemy.flyer.count;
}

int Config::get_flyer_life(){
    return enemy.flyer.life;
}

float Config::get_flyer_speed(){
    return enemy.flyer.speed;
}

int Config::get_flyer_damage(){
    return enemy.flyer.damage;
}

//shooter
int Config::get_shooter_count(){
    return enemy.shooter.count;
}

int Config::get_shooter_life(){
    return enemy.shooter.life;
}

float Config::get_shooter_speed(){
    return enemy.shooter.speed;
}

int Config::get_shooter_damage(){
    return enemy.shooter.damage;
}

//crawler
int Config::get_crawler_count(){
    return enemy.crawler.count;
}

int Config::get_crawler_life(){
    return enemy.crawler.life;
}

float Config::get_crawler_speed(){
    return enemy.crawler.speed;
}

int Config::get_crawler_damage(){
    return enemy.crawler.damage;
}

//Player
float Config::get_player_speed(){
    return player.speed;
}

int Config::get_player_jump(){
    return player.jump;
}

int Config::get_player_max_health(){
    return player.max_health;
}
