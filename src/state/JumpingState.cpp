#include "state/JumpingState.hpp"
#include "entity/Player.hpp"
#include "input/MoveLeftCommand.hpp"
#include "input/MoveRightCommand.hpp"
#include "input/IdleCommand.hpp"
#include "state/IdleState.hpp"
#include "state/RunningState.hpp"

namespace vertex {

void JumpingState::enter(Player& player) {
    (void)player;
}

void JumpingState::update(Player& player, float dt) {
    (void)dt;
    if (player.isGrounded()) {
        if (player.getVelocityX() != 0.0f) {
            player.setState(std::make_unique<RunningState>());
        } else {
            player.setState(std::make_unique<IdleState>());
        }
    }
}

void JumpingState::handleInput(Player& player, Command& command) {
    if (dynamic_cast<MoveLeftCommand*>(&command)) {
        player.setTargetVelocityX(-Player::RUN_SPEED);
    } else if (dynamic_cast<MoveRightCommand*>(&command)) {
        player.setTargetVelocityX(Player::RUN_SPEED);
    } else if (dynamic_cast<IdleCommand*>(&command)) {
        player.setTargetVelocityX(0.0f);
    }
}

}  // namespace vertex
