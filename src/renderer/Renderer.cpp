#include "renderer/Renderer.hpp"
#include "renderer/BackBuffer.hpp"
#include "renderer/Sprite.hpp"
#include "game/GameWorld.hpp"
#include "entity/Player.hpp"
#include <curses.h>
#include <algorithm>

namespace vertex {

Renderer::Renderer() {
    getmaxyx(stdscr, height_, width_);
    if (height_ <= 0) height_ = 24;
    if (width_ <= 0) width_ = 80;
    buffer_ = std::make_unique<BackBuffer>(width_, height_);
}

Renderer::~Renderer() = default;

void Renderer::clear() {
    buffer_->clear(' ');
}

void Renderer::draw(const GameWorld& world) {
    const int ground_row = height_ - 2;
    const float camera_x = world.getCameraX();

    for (int x = 0; x < width_; ++x) {
        buffer_->put(x, ground_row, '_');
    }

    for (const auto& obs : world.getObstacles()) {
        const int screen_x = static_cast<int>(obs.x - camera_x);
        if (screen_x + obs.width <= 0 || screen_x >= width_) continue;

        const int obs_top = static_cast<int>(obs.y);
        for (int dy = 0; dy < obs.height; ++dy) {
            for (int dx = 0; dx < obs.width; ++dx) {
                const int sx = screen_x + dx;
                if (sx >= 0 && sx < width_ && obs_top + dy < height_) {
                    buffer_->put(sx, obs_top + dy, '#');
                }
            }
        }
    }

    const auto* player = world.getPlayer();
    if (!player) return;

    const auto& sprite = player->getSprite();
    auto pos = player->getPosition();
    int x = static_cast<int>(pos.x - camera_x);
    int y = static_cast<int>(pos.y);

    if (x + Player::SPRITE_WIDTH > 0 && x < width_) {
        buffer_->putSprite(x, y, sprite.lines);
    }
}

void Renderer::present() {
    const auto& data = buffer_->data();
    for (int y = 0; y < height_ && y < static_cast<int>(data.size()); ++y) {
        mvaddnstr(y, 0, data[static_cast<size_t>(y)].c_str(),
                  std::min(static_cast<int>(data[static_cast<size_t>(y)].size()), width_));
    }
    refresh();
}

}  // namespace vertex
