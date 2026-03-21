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

    const float max_x = static_cast<float>(world.getWorldWidth() - 3);
    pos.x = std::clamp(pos.x, 0.0f, max_x);

    player->setPosition(pos);
    player->setVelocityY(vy);
}

}  // namespace vertex
