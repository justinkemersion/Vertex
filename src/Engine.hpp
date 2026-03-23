#pragma once

#include <memory>

namespace vertex {

class GameLoop;
class Renderer;
class InputHandler;
class PhysicsEngine;
class GameWorld;

class Engine {
public:
    Engine();
    ~Engine();

    void run();

    bool isRunning() const { return running_; }
    void processInput(float dt);
    void update(float dt);
    void render();

private:
    void initNcurses();
    void shutdownNcurses();
    static bool isMovementKey(int key);

    bool running_{true};
    float time_since_movement_{0.0f};
    bool movement_held_{false};
    static constexpr float TAP_IDLE_TIME = 0.55f;
    static constexpr float HELD_IDLE_TIME = 0.20f;
    static constexpr float MAX_REPEAT_GAP = 0.15f;
    std::unique_ptr<GameLoop> gameLoop_;
    std::unique_ptr<Renderer> renderer_;
    std::unique_ptr<InputHandler> inputHandler_;
    std::unique_ptr<PhysicsEngine> physicsEngine_;
    std::unique_ptr<GameWorld> gameWorld_;
};

}  // namespace vertex
