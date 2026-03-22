#include "physics/PhysicsEngine.hpp"
#include "game/GameWorld.hpp"
#include "entity/Player.hpp"
#include <algorithm>

namespace vertex {

void PhysicsEngine::update(GameWorld& world, float dt) {
    Player* player = world.getPlayer();
    if (!player) return;

    auto pos = player->getPosition();
    float vx = player->getVelocityX();
    float vy = player->getVelocityY();
    const float target_vx = player->getTargetVelocityX();

    if (player->isGrounded()) {
        const float accel = (std::abs(target_vx) > std::abs(vx)) ? Player::RUN_ACCELERATION : Player::GROUND_FRICTION;
        const float diff = target_vx - vx;
        if (std::abs(diff) < accel * dt) {
            vx = target_vx;
        } else {
            vx += (diff > 0 ? 1.0f : -1.0f) * accel * dt;
        }
    } else {
        const float air_accel = Player::RUN_ACCELERATION * Player::AIR_CONTROL;
        const float diff = target_vx - vx;
        if (std::abs(diff) < air_accel * dt) {
            vx = target_vx;
        } else {
            vx += (diff > 0 ? 1.0f : -1.0f) * air_accel * dt;
        }
    }

    vy += Player::GRAVITY * dt;
    const float prev_x = pos.x;
    const float prev_y = pos.y;
    pos.x += vx * dt;
    pos.y += vy * dt;

    const float min_y = 0.0f;
    const float ground_level = static_cast<float>(world.getScreenHeight() - Player::SPRITE_HEIGHT - 2);

    if (pos.y >= ground_level) {
        pos.y = ground_level;
        vy = 0.0f;
        player->setGrounded(true);
    } else {
        player->setGrounded(false);
        if (pos.y < min_y) {
            pos.y = min_y;
            vy = 0.0f;
        }
    }

    const float pw = static_cast<float>(Player::SPRITE_WIDTH);
    const float ph = static_cast<float>(Player::SPRITE_HEIGHT);

    for (const auto& obs : world.getObstacles()) {
        const float pr = pos.x + pw;
        const float pb = pos.y + ph;
        const float or_ = obs.right();
        const float ob = obs.bottom();

        if (pos.x < or_ && pr > obs.x && pos.y < ob && pb > obs.y) {
            constexpr float landing_tolerance = 0.5f;

            const bool was_above = prev_y + ph <= obs.y + landing_tolerance;

            // Wall-stop only when on ground or already falling. While rising (or at jump apex),
            // do not kill horizontal speed or shove the player back — that made jump-over require
            // perfect timing and felt like an invisible wall.
            const bool wall_stop_horizontal = player->isGrounded() || vy > 0.f;

            if (was_above && vy >= 0.f) {
                pos.y = obs.y - ph;
                vy = 0.f;
                player->setGrounded(true);
            } else if (prev_y >= ob && vy < 0.f) {
                pos.y = ob;
                vy = 0.f;
            } else if (wall_stop_horizontal && prev_x + pw <= obs.x && vx > 0.f) {
                pos.x = obs.x - pw;
                vx = 0.f;
            } else if (wall_stop_horizontal && prev_x >= or_ && vx < 0.f) {
                pos.x = or_;
                vx = 0.f;
            }
        }
    }

    const float max_x = static_cast<float>(world.getLevelWidth() - Player::SPRITE_WIDTH);
    pos.x = std::clamp(pos.x, 0.0f, max_x);

    player->setPosition(pos);
    player->setVelocityX(vx);
    player->setVelocityY(vy);
}

}  // namespace vertex
