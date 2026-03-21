#pragma once

#include "input/Command.hpp"

namespace vertex {

class JumpCommand : public Command {
public:
    void execute(Player& player) override;
};

}  // namespace vertex
