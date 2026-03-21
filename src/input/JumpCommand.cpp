#include "input/JumpCommand.hpp"
#include "entity/Player.hpp"

namespace vertex {

void JumpCommand::execute(Player& player) {
    player.getState()->handleInput(player, *this);
}

}  // namespace vertex
