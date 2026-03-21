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
    void processInput();
    void update(float dt);
    void render();

private:
    void initNcurses();
    void shutdownNcurses();

    bool running_{true};
    std::unique_ptr<GameLoop> gameLoop_;
    std::unique_ptr<Renderer> renderer_;
    std::unique_ptr<InputHandler> inputHandler_;
    std::unique_ptr<PhysicsEngine> physicsEngine_;
    std::unique_ptr<GameWorld> gameWorld_;
};

}  // namespace vertex
