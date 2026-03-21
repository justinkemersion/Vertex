#pragma once

namespace vertex {

class Player;

class Command {
public:
    virtual ~Command() = default;
    virtual void execute(Player& player) = 0;
};

}  // namespace vertex
