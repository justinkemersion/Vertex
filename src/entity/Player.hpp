#pragma once

#include "entity/Entity.hpp"
#include "renderer/Sprite.hpp"
#include "state/PlayerState.hpp"
#include <memory>

namespace vertex {

class Player : public Entity {
public:
    static constexpr float GRAVITY = 400.0f;
    static constexpr float RUN_SPEED = 150.0f;
    static constexpr float JUMP_VELOCITY = -280.0f;
    static constexpr int SPRITE_HEIGHT = 3;

    Player();

    const Sprite& getSprite() const override;

    void setState(std::unique_ptr<PlayerState> state);
    PlayerState* getState() { return state_.get(); }

    float getVelocityX() const { return velocity_x_; }
    float getVelocityY() const { return velocity_y_; }
    void setVelocityX(float v) { velocity_x_ = v; }
    void setVelocityY(float v) { velocity_y_ = v; }

    bool isGrounded() const { return grounded_; }
    void setGrounded(bool g) { grounded_ = g; }

private:
    std::unique_ptr<PlayerState> state_;
    float velocity_x_{0.0f};
    float velocity_y_{0.0f};
    bool grounded_{true};
    Sprite sprite_;
};

}  // namespace vertex
