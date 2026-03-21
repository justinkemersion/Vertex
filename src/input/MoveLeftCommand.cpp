#include "input/MoveLeftCommand.hpp"
#include "entity/Player.hpp"

namespace vertex {

void MoveLeftCommand::execute(Player& player) {
    player.getState()->handleInput(player, *this);
}

}  // namespace vertex
