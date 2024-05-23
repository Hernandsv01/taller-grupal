#include <iostream>
#include <optional>

#include "gui_loop.cpp"

int main() {
    std::optional a = nullptr;

    std::cout << "Hola mundo." << std::endl;

    gui_hello_world();

    return 0;
}