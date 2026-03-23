#include "entity/Player.hpp"
#include "state/IdleState.hpp"
#include "state/JumpingState.hpp"
#include "state/RunningState.hpp"
#include <cmath>

namespace vertex {

Player::Player() {
    position_.x = 5.0f;
    position_.y = 19.0f;
    setState(std::make_unique<IdleState>());
    sprite_idle_.lines = {
        " O ",
        "/|\\",
        "/ \\"
    };
    sprite_run_1_.lines = {
        " O ",
        "/|\\",
        "/ \\"   // feet normal
    };
    sprite_run_2_.lines = {
        " O ",
        "/|\\",
        "\\ /"   // feet crossed - stride
    };
}

const Sprite& Player::getSprite() const {
    const bool show_run = dynamic_cast<const RunningState*>(getState()) != nullptr ||
        (dynamic_cast<const JumpingState*>(getState()) != nullptr &&
         std::abs(getTargetVelocityX()) > 0.1f);
    if (show_run) {
        const int frame = static_cast<int>(run_animation_time_ * 14.0f) % 2;
        return (frame == 0) ? sprite_run_1_ : sprite_run_2_;
    }
    return sprite_idle_;
}

void Player::advanceRunAnimation(float dt) {
    run_animation_time_ += dt;
}

void Player::setState(std::unique_ptr<PlayerState> state) {
    if (state) {
        state_ = std::move(state);
        state_->enter(*this);
    }
}

}  // namespace vertex
