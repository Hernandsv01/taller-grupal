#ifndef CONFIG_LOADER_H
#define CONFIG_LOADER_H

#include <yaml-cpp/yaml.h>
#include <iostream>

#define CONFIG_PATH "config.yaml"

class ConfigLoader{
public: 
    //cargar desde un archivo predeterminado algo, como respuestas me deberia generar 
    //un dato de configuracion que permitirian que se configure en la creacion del Game.
    //Capaz el constructor del juego debe recibir esta configucarion y cuando se crea 
    //la partida se carga con ella.
    static void load_config(const std::string& file_path);

};

#endif // CONFIG_LOADER_H