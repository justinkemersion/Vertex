#pragma once

#include "input/Command.hpp"

namespace vertex {

class MoveLeftCommand : public Command {
public:
    void execute(Player& player) override;
};

}  // namespace vertex
