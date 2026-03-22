#pragma once

#include "entity/Player.hpp"
#include "game/Obstacle.hpp"
#include "physics/PhysicsEngine.hpp"
#include <memory>
#include <vector>

namespace vertex {

class GameWorld {
public:
    GameWorld();

    Player* getPlayer() { return player_.get(); }
    const Player* getPlayer() const { return player_.get(); }
    PhysicsEngine& getPhysics() { return physics_; }

    int getScreenWidth() const { return screen_width_; }
    int getScreenHeight() const { return screen_height_; }
    void setWorldSize(int w, int h);

    int getLevelWidth() const { return level_width_; }
    float getCameraX() const { return camera_x_; }
    void updateCamera();
    void buildLevel();

    const std::vector<Obstacle>& getObstacles() const { return obstacles_; }

private:

    std::unique_ptr<Player> player_;
    PhysicsEngine physics_;
    int screen_width_{80};
    int screen_height_{24};
    int level_width_{400};
    float camera_x_{0.0f};
    std::vector<Obstacle> obstacles_;
};

}  // namespace vertex
