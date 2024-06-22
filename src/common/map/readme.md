Para poder usar yaml-cpp, hay que instalar
```bash
sudo apt-get install libyaml-cpp-dev
```

Y luego incluir en el CMakeLists.txt la linea
```cmake
target_link_libraries(main.out PUBLIC yaml-cpp)
```