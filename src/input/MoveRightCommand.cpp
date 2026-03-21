#include "input/MoveRightCommand.hpp"
#include "entity/Player.hpp"

namespace vertex {

void MoveRightCommand::execute(Player& player) {
    player.getState()->handleInput(player, *this);
}

}  // namespace vertex
