#include "entity/Player.hpp"
#include "state/IdleState.hpp"

namespace vertex {

Player::Player() {
    position_.x = 5.0f;
    position_.y = 19.0f;
    setState(std::make_unique<IdleState>());
    sprite_.lines = {
        " o ",
        "/|\\",
        "/ \\"
    };
}

const Sprite& Player::getSprite() const {
    return sprite_;
}

void Player::setState(std::unique_ptr<PlayerState> state) {
    if (state) {
        state_ = std::move(state);
        state_->enter(*this);
    }
}

}  // namespace vertex
