#include "state/JumpingState.hpp"
#include "entity/Player.hpp"
#include <cmath>
#include "input/MoveLeftCommand.hpp"
#include "input/MoveRightCommand.hpp"
#include "input/IdleCommand.hpp"
#include "state/IdleState.hpp"
#include "state/RunningState.hpp"

namespace vertex {

void JumpingState::enter(Player& player) {
    // Preserve horizontal momentum into the jump — do not clear target_velocity_x.
    // Physics will apply air control toward target; Engine skips auto-idle when airborne.
    (void)player;
}

void JumpingState::update(Player& player, float dt) {
    if (std::abs(player.getTargetVelocityX()) > 0.1f) {
        player.advanceRunAnimation(dt);
    }
    if (player.isGrounded()) {
        if (std::abs(player.getTargetVelocityX()) > 0.1f) {
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
