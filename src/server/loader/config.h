#ifndef CONFIG_H
#define CONFIG_H

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
public:
  static GameConfig game;
  static EnemyConfig enemy;
  static PlayerConfig player;  
};


#endif //CONFIG_H
