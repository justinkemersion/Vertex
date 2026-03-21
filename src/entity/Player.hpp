#pragma once

#include "entity/Entity.hpp"
#include "renderer/Sprite.hpp"
#include "state/PlayerState.hpp"
#include <memory>

namespace vertex {

class Player : public Entity {
public:
    static constexpr float GRAVITY = 320.0f;
    static constexpr float RUN_SPEED = 85.0f;
    static constexpr float RUN_ACCELERATION = 450.0f;
    static constexpr float GROUND_FRICTION = 520.0f;
    static constexpr float AIR_CONTROL = 0.6f;
    static constexpr float JUMP_VELOCITY = -240.0f;
    static constexpr int SPRITE_HEIGHT = 3;
    static constexpr int SPRITE_WIDTH = 5;

    Player();

    const Sprite& getSprite() const override;

    void setState(std::unique_ptr<PlayerState> state);
    PlayerState* getState() { return state_.get(); }
    const PlayerState* getState() const { return state_.get(); }

    float getVelocityX() const { return velocity_x_; }
    float getVelocityY() const { return velocity_y_; }
    void setVelocityX(float v) { velocity_x_ = v; }
    void setVelocityY(float v) { velocity_y_ = v; }

    float getTargetVelocityX() const { return target_velocity_x_; }
    void setTargetVelocityX(float v) { target_velocity_x_ = v; }

    bool isGrounded() const { return grounded_; }
    void setGrounded(bool g) { grounded_ = g; }

    void advanceRunAnimation(float dt);

private:
    std::unique_ptr<PlayerState> state_;
    float velocity_x_{0.0f};
    float velocity_y_{0.0f};
    float target_velocity_x_{0.0f};
    bool grounded_{true};
    float run_animation_time_{0.0f};
    Sprite sprite_idle_;
    Sprite sprite_run_1_;
    Sprite sprite_run_2_;
};

}  // namespace vertex
