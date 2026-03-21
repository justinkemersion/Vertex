#pragma once

namespace vertex {

class GameWorld;

class PhysicsEngine {
public:
    void update(GameWorld& world, float dt);
};

}  // namespace vertex
