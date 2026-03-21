#include "input/IdleCommand.hpp"
#include "entity/Player.hpp"

namespace vertex {

void IdleCommand::execute(Player& player) {
    player.getState()->handleInput(player, *this);
}

}  // namespace vertex
