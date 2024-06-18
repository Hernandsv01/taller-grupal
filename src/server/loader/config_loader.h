#ifndef CONFIG_LOADER_H
#define CONFIG_LOADER_H

#include "yaml-cpp/yaml.h"

//Deberia revisar las entidades y ver que otras cuestiones se puede configurar 


// struct Config{
//     GameConfig game;
//     EnemyConfig enemy;
//     PlayerConfig player;
//     AssetsConfig assets;
// }

class ConfigLoader{
private:

public: 
    //cargar desde un archivo predeterminado algo, como respuestas me deberia generar 
    //un dato de configuracion que permitirian que se configure en la creacion del Game.
    //Capaz el constructor del juego debe recibir esta configucarion y cuando se crea 
    //la partida se carga con ella.
    static int load(std::string file_name);

    // lectura del yaml especificamente... 

};

#endif // CONFIG_LOADER_H