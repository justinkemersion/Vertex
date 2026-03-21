#include "input/InputHandler.hpp"
#include "input/MoveLeftCommand.hpp"
#include "input/MoveRightCommand.hpp"
#include "input/JumpCommand.hpp"
#include "input/IdleCommand.hpp"
#include "entity/Player.hpp"
#include <curses.h>

namespace vertex {

InputHandler::InputHandler() {
    setupKeyBindings();
}

void InputHandler::setupKeyBindings() {
    keyBindings_[KEY_LEFT] = std::make_unique<MoveLeftCommand>();
    keyBindings_[static_cast<int>('a')] = std::make_unique<MoveLeftCommand>();
    keyBindings_[KEY_RIGHT] = std::make_unique<MoveRightCommand>();
    keyBindings_[static_cast<int>('d')] = std::make_unique<MoveRightCommand>();
    keyBindings_[static_cast<int>(' ')] = std::make_unique<JumpCommand>();
    keyBindings_[KEY_UP] = std::make_unique<JumpCommand>();
    keyBindings_[KEY_DOWN] = std::make_unique<IdleCommand>();
    keyBindings_[static_cast<int>('s')] = std::make_unique<IdleCommand>();
}

void InputHandler::handleInput(int key, Player& player) {
    auto it = keyBindings_.find(key);
    if (it != keyBindings_.end() && it->second) {
        it->second->execute(player);
    }
}

}  // namespace vertex
