#ifndef CONFIG_H
#define CONFIG_H


#include <yaml-cpp/yaml.h>
#include <iostream>

#define CONFIG_PATH "config.yaml"

struct GameConfig{
    int time = 15;

};

struct PickupsConfig{
   int coin = 50;
   int carrot = 25;
   int light_ammo = 100;
   int heavy_ammo = 25;
   int power_ammo = 10;
};


struct BulletsConfig{
    struct Attributes{
        int max_ammo;
        int speed; 
        int damage; 
        int bpm;
        Attributes(int max_ammo,int speed, int damage, int bpm) : max_ammo(max_ammo), speed(speed), damage(damage), bpm(bpm){};
    };
    
    Attributes normal = Attributes(-1, 5, 20, 500);
    Attributes light = Attributes(1000,20, 10, 3);
    Attributes heavy = Attributes(50,20, 10, 3);
    Attributes power = Attributes(10,20, 10, 3);
};

struct EnemyConfig{
    struct Attributes{
        int count;
        int life;
        int speed;
        int damage;
        Attributes(int count, int life, int speed, int damage) : count(count), life(life), speed(speed), damage(damage) {};
    };
    Attributes flyer = Attributes(5, 100, 10, 10);
    Attributes shooter = Attributes(5, 100, 10, 10);
    Attributes crawler = Attributes(5, 100, 10, 10);
};

struct PlayerConfig{
    int speed = 10;
    int jump = 15;
    int max_health = 100;
};

class Config{
private:
  static GameConfig game;
  static PickupsConfig pickups;
  static BulletsConfig bullet;
  static EnemyConfig enemy;
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
