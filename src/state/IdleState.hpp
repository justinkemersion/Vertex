#pragma once

#include "state/PlayerState.hpp"

namespace vertex {

class IdleState : public PlayerState {
public:
    void enter(Player& player) override;
    void update(Player& player, float dt) override;
    void handleInput(Player& player, Command& command) override;
};

}  // namespace vertex
