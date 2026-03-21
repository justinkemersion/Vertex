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
    pos.x += vx * dt;
    pos.y += vy * dt;

    const float min_y = 0.0f;
    const float ground_level = static_cast<float>(world.getWorldHeight() - Player::SPRITE_HEIGHT - 2);

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

    const float max_x = static_cast<float>(world.getWorldWidth() - Player::SPRITE_WIDTH);
    pos.x = std::clamp(pos.x, 0.0f, max_x);

    player->setPosition(pos);
    player->setVelocityX(vx);
    player->setVelocityY(vy);
}

}  // namespace vertex
