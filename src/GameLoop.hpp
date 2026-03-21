#pragma once

#include <chrono>
#include <memory>

namespace vertex {

class Engine;

class GameLoop {
public:
    explicit GameLoop(Engine& engine);

    void run();

private:
    Engine& engine_;
    static constexpr double TARGET_DT = 1.0 / 60.0;
    using Clock = std::chrono::steady_clock;
};

}  // namespace vertex
