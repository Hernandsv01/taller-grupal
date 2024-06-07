#include <iostream>
#include <optional>
#include <vector>

#include "gui_loop.h"

int main() {
    std::optional a = nullptr;

    std::cout << "Hola mundo." << std::endl;

    GuiLoop loop;

    loop.start();

    std::this_thread::sleep_for(std::chrono::seconds(5));

    loop.stop();
    loop.join();

    return 0;
}