#include "game/GameWorld.hpp"
#include "entity/Player.hpp"
#include <algorithm>

namespace vertex {

GameWorld::GameWorld() : player_(std::make_unique<Player>()) {}

void GameWorld::setWorldSize(int w, int h) {
    screen_width_ = w;
    screen_height_ = h;
    buildLevel();
}

void GameWorld::updateCamera() {
    const auto* player = getPlayer();
    if (!player) return;

    const float px = player->getPosition().x;
    const float margin = static_cast<float>(screen_width_) * 0.35f;

    if (px > camera_x_ + margin) {
        camera_x_ = px - margin;
    }
    if (px < camera_x_ + static_cast<float>(screen_width_) * 0.2f) {
        camera_x_ = px - static_cast<float>(screen_width_) * 0.2f;
    }

    camera_x_ = std::max(0.0f, camera_x_);
    camera_x_ = std::min(camera_x_, static_cast<float>(level_width_ - screen_width_));
}

void GameWorld::buildLevel() {
    const float ground = static_cast<float>(screen_height_ - 2 - 1);

    obstacles_.clear();
    obstacles_.push_back({30.0f, ground, 3, 1});
    obstacles_.push_back({55.0f, ground, 2, 1});
    obstacles_.push_back({90.0f, ground, 4, 1});
    obstacles_.push_back({130.0f, ground, 2, 1});
    obstacles_.push_back({170.0f, ground, 3, 1});
    obstacles_.push_back({220.0f, ground, 5, 1});
    obstacles_.push_back({270.0f, ground, 2, 1});
    obstacles_.push_back({310.0f, ground, 4, 1});
}

}  // namespace vertex
