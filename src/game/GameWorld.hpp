#pragma once

#include "entity/Player.hpp"
#include "physics/PhysicsEngine.hpp"
#include <memory>

namespace vertex {

class GameWorld {
public:
    GameWorld();

    Player* getPlayer() { return player_.get(); }
    const Player* getPlayer() const { return player_.get(); }
    PhysicsEngine& getPhysics() { return physics_; }

    int getWorldWidth() const { return world_width_; }
    int getWorldHeight() const { return world_height_; }
    void setWorldSize(int w, int h) { world_width_ = w; world_height_ = h; }

private:
    std::unique_ptr<Player> player_;
    PhysicsEngine physics_;
    int world_width_{80};
    int world_height_{24};
};

}  // namespace vertex
