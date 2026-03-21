#include "GameLoop.hpp"
#include "Engine.hpp"
#include <curses.h>
#include <thread>

namespace vertex {

GameLoop::GameLoop(Engine& engine) : engine_(engine) {}

void GameLoop::run() {
    auto last_frame = Clock::now();

    while (engine_.isRunning()) {
        auto now = Clock::now();
        float dt = std::chrono::duration<float>(now - last_frame).count();
        last_frame = now;

        if (dt > 0.25f) dt = 0.25f;

        engine_.processInput();
        engine_.update(dt);
        engine_.render();

        auto elapsed = std::chrono::duration<double>(Clock::now() - now).count();
        if (elapsed < TARGET_DT) {
            std::this_thread::sleep_for(
                std::chrono::duration<double>(TARGET_DT - elapsed));
        }
    }
}

}  // namespace vertex
