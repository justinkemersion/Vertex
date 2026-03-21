#include "state/RunningState.hpp"
#include "entity/Player.hpp"
#include "input/MoveLeftCommand.hpp"
#include "input/MoveRightCommand.hpp"
#include "input/IdleCommand.hpp"
#include "input/JumpCommand.hpp"
#include "state/IdleState.hpp"
#include "state/JumpingState.hpp"

namespace vertex {

void RunningState::enter(Player& player) {
    (void)player;
}

void RunningState::update(Player& player, float dt) {
    (void)player;
    (void)dt;
}

void RunningState::handleInput(Player& player, Command& command) {
    if (dynamic_cast<MoveLeftCommand*>(&command)) {
        player.setVelocityX(-Player::RUN_SPEED);
    } else if (dynamic_cast<MoveRightCommand*>(&command)) {
        player.setVelocityX(Player::RUN_SPEED);
    } else if (dynamic_cast<IdleCommand*>(&command)) {
        player.setState(std::make_unique<IdleState>());
    } else if (dynamic_cast<JumpCommand*>(&command) && player.isGrounded()) {
        player.setState(std::make_unique<JumpingState>());
        player.setVelocityY(Player::JUMP_VELOCITY);
        player.setGrounded(false);
    }
}

}  // namespace vertex
