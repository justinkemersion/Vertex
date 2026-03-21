#include "game/GameWorld.hpp"

namespace vertex {

GameWorld::GameWorld() : player_(std::make_unique<Player>()) {}

}  // namespace vertex
