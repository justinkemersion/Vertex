#include "Engine.hpp"
#include <curses.h>
#include <exception>
#include <iostream>
#include <memory>

int main() {
    try {
        auto engine = std::make_unique<vertex::Engine>();
        engine->run();
    } catch (const std::exception& e) {
        endwin();
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
