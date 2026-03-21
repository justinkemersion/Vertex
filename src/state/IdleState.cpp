#include "state/IdleState.hpp"
#include "entity/Player.hpp"
#include "input/MoveLeftCommand.hpp"
#include "input/MoveRightCommand.hpp"
#include "input/JumpCommand.hpp"
#include "state/RunningState.hpp"
#include "state/JumpingState.hpp"

namespace vertex {

void IdleState::enter(Player& player) {
    player.setTargetVelocityX(0.0f);
}

void IdleState::update(Player& player, float dt) {
    (void)dt;
    if (!player.isGrounded()) {
        player.setTargetVelocityX(0.0f);
    }
}

void IdleState::handleInput(Player& player, Command& command) {
    if (dynamic_cast<MoveLeftCommand*>(&command)) {
        player.setState(std::make_unique<RunningState>());
        player.setTargetVelocityX(-Player::RUN_SPEED);
    } else if (dynamic_cast<MoveRightCommand*>(&command)) {
        player.setState(std::make_unique<RunningState>());
        player.setTargetVelocityX(Player::RUN_SPEED);
    } else if (dynamic_cast<JumpCommand*>(&command) && player.isGrounded()) {
        player.setState(std::make_unique<JumpingState>());
        player.setVelocityY(Player::JUMP_VELOCITY);
        player.setGrounded(false);
    }
}

}  // namespace vertex
