#pragma once

namespace vertex {

class Player;
class Command;

class PlayerState {
public:
    virtual ~PlayerState() = default;

    virtual void enter(Player& player) = 0;
    virtual void update(Player& player, float dt) = 0;
    virtual void handleInput(Player& player, Command& command) = 0;
};

}  // namespace vertex
