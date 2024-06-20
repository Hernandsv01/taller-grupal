#ifndef CONFIG_H
#define CONFIG_H


#include <yaml-cpp/yaml.h>
#include <iostream>

#define CONFIG_PATH "config.yaml"

struct GameConfig{
    uint8_t time = 0;

};

struct EnemyConfig{
    struct Attributes{
        int count = 5;
        int life = 100;
        int speed = 10;
        int damage= 10;
    };
    Attributes flyer;
    Attributes shooter;
    Attributes crawler;
};

struct PlayerConfig{
    int speed = 10;
    int jump = 15;
    int max_health = 100;
};

class Config{
private:
  static GameConfig game;
//   static EnemyConfig enemy;
  static PlayerConfig player;  

public: 

    //cargar desde un archivo predeterminado algo, como respuestas me deberia generar 
    //un dato de configuracion que permitirian que se configure en la creacion del Game.
    //Capaz el constructor del juego debe recibir esta configucarion y cuando se crea 
    //la partida se carga con ella.
    static void load_config(const std::string& file_path);

    static int getPlayerSpeed(){
        return player.speed;
    }



    
};


#endif //CONFIG_H
