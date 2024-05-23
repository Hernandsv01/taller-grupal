#include <iostream>
#include <optional>
#include <vector>

#include "gui_loop.h"

int main() {
    std::optional a = nullptr;

    std::cout << "Hola mundo." << std::endl;

    GuiLoop loop;

    loop.ejecutar();

    return 0;
}