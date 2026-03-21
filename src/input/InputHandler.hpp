#pragma once

#include "input/Command.hpp"
#include <memory>
#include <unordered_map>

namespace vertex {

class Player;

class InputHandler {
public:
    InputHandler();
    void handleInput(int key, Player& player);

private:
    std::unordered_map<int, std::unique_ptr<Command>> keyBindings_;
    void setupKeyBindings();
};

}  // namespace vertex
