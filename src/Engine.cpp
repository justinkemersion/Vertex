#include "Engine.hpp"
#include "GameLoop.hpp"
#include "renderer/Renderer.hpp"
#include "input/InputHandler.hpp"
#include "physics/PhysicsEngine.hpp"
#include "game/GameWorld.hpp"
#include "entity/Player.hpp"
#include "physics/Vector2.hpp"
#include <curses.h>
#include <cerrno>
#include <cstring>

namespace vertex {

Engine::Engine() {
    initNcurses();
    gameLoop_ = std::make_unique<GameLoop>(*this);
    renderer_ = std::make_unique<Renderer>();
    inputHandler_ = std::make_unique<InputHandler>();
    physicsEngine_ = std::make_unique<PhysicsEngine>();
    gameWorld_ = std::make_unique<GameWorld>();
    int rows = 0, cols = 0;
    getmaxyx(stdscr, rows, cols);
    if (rows > 0 && cols > 0) {
        gameWorld_->setWorldSize(cols, rows);
        if (auto* player = gameWorld_->getPlayer()) {
            Vector2 pos = player->getPosition();
            pos.y = static_cast<float>(rows - Player::SPRITE_HEIGHT - 2);
            player->setPosition(pos);
        }
    }
}

Engine::~Engine() {
    shutdownNcurses();
}

void Engine::initNcurses() {
    if (initscr() == nullptr) {
        throw std::runtime_error(std::string("initscr failed: ") + std::strerror(errno));
    }
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    nodelay(stdscr, TRUE);
    timeout(0);
}

void Engine::shutdownNcurses() {
    endwin();
}

void Engine::run() {
    gameLoop_->run();
}

bool Engine::isMovementKey(int key) {
    return key == KEY_LEFT || key == KEY_RIGHT ||
           key == 'a' || key == 'A' || key == 'd' || key == 'D';
}

void Engine::processInput(float dt) {
    auto* player = gameWorld_->getPlayer();
    bool got_movement = false;
    int horizontal_key = ERR;
    bool jump_requested = false;
    bool idle_requested = false;

    int ch;
    while ((ch = getch()) != ERR) {
        if (ch == 'q' || ch == 'Q') {
            running_ = false;
            return;
        }

        if (isMovementKey(ch)) {
            horizontal_key = ch;
            got_movement = true;
        } else if (ch == KEY_UP || ch == static_cast<int>(' ')) {
            jump_requested = true;
        } else if (ch == KEY_DOWN || ch == static_cast<int>('s') || ch == static_cast<int>('S')) {
            idle_requested = true;
        }
    }

    if (!player) return;

    if (horizontal_key != ERR) {
        inputHandler_->handleInput(horizontal_key, *player);
    }
    if (jump_requested) {
        inputHandler_->handleInput(KEY_UP, *player);
    } else if (idle_requested && horizontal_key == ERR) {
        inputHandler_->handleInput(KEY_DOWN, *player);
    }

    if (got_movement) {
        if (time_since_movement_ > 0.0f) {
            movement_held_ = true;
        }
        time_since_movement_ = 0.0f;
    } else {
        time_since_movement_ += dt;
        const float threshold = movement_held_ ? HELD_IDLE_TIME : TAP_IDLE_TIME;
        if (time_since_movement_ >= threshold && player->getTargetVelocityX() != 0.0f &&
            player->isGrounded() && !jump_requested) {
            player->setTargetVelocityX(0.0f);
            movement_held_ = false;
        }
    }
}

void Engine::update(float dt) {
    physicsEngine_->update(*gameWorld_, dt);
    gameWorld_->updateCamera();
    if (auto* player = gameWorld_->getPlayer()) {
        if (player->getState()) {
            player->getState()->update(*player, dt);
        }
    }
}

void Engine::render() {
    renderer_->clear();
    renderer_->draw(*gameWorld_);
    renderer_->present();
}

}  // namespace vertex
