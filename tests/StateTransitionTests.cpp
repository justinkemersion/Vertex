#include <doctest/doctest.h>

#include "game/GameWorld.hpp"
#include "entity/Player.hpp"
#include "input/MoveLeftCommand.hpp"
#include "input/MoveRightCommand.hpp"
#include "input/IdleCommand.hpp"
#include "input/JumpCommand.hpp"
#include "state/IdleState.hpp"
#include "state/RunningState.hpp"
#include "state/JumpingState.hpp"

using namespace vertex;

TEST_CASE("State transition: Idle + MoveLeft -> Running") {
    GameWorld world;
    world.setWorldSize(80, 24);
    Player* player = world.getPlayer();
    REQUIRE(player != nullptr);

    CHECK(dynamic_cast<IdleState*>(player->getState()) != nullptr);

    MoveLeftCommand cmd;
    cmd.execute(*player);

    CHECK(dynamic_cast<RunningState*>(player->getState()) != nullptr);
    CHECK(player->getTargetVelocityX() == doctest::Approx(-Player::RUN_SPEED));
}

TEST_CASE("State transition: Idle + MoveRight -> Running") {
    GameWorld world;
    world.setWorldSize(80, 24);
    Player* player = world.getPlayer();
    REQUIRE(player != nullptr);

    MoveRightCommand cmd;
    cmd.execute(*player);

    CHECK(dynamic_cast<RunningState*>(player->getState()) != nullptr);
    CHECK(player->getTargetVelocityX() == doctest::Approx(Player::RUN_SPEED));
}

TEST_CASE("State transition: Running + Idle -> Idle") {
    GameWorld world;
    world.setWorldSize(80, 24);
    Player* player = world.getPlayer();
    REQUIRE(player != nullptr);

    MoveLeftCommand move_cmd;
    move_cmd.execute(*player);
    CHECK(dynamic_cast<RunningState*>(player->getState()) != nullptr);

    IdleCommand idle_cmd;
    idle_cmd.execute(*player);

    CHECK(dynamic_cast<IdleState*>(player->getState()) != nullptr);
    CHECK(player->getTargetVelocityX() == doctest::Approx(0.0f));
}

TEST_CASE("State transition: Idle + Jump when grounded -> Jumping") {
    GameWorld world;
    world.setWorldSize(80, 24);
    Player* player = world.getPlayer();
    REQUIRE(player != nullptr);
    player->setGrounded(true);

    JumpCommand cmd;
    cmd.execute(*player);

    CHECK(dynamic_cast<JumpingState*>(player->getState()) != nullptr);
    CHECK(player->getVelocityY() == doctest::Approx(Player::JUMP_VELOCITY));
    CHECK(player->isGrounded() == false);
}
