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

TEST_CASE("Movement from idle uses acceleration, not instant snap") {
    GameWorld world;
    world.setWorldSize(80, 24);
    Player* player = world.getPlayer();
    REQUIRE(player != nullptr);

    CHECK(player->getVelocityX() == doctest::Approx(0.0f));

    MoveRightCommand cmd;
    cmd.execute(*player);

    CHECK(player->getTargetVelocityX() == doctest::Approx(Player::RUN_SPEED));
    CHECK(player->getVelocityX() == doctest::Approx(0.0f));
}

TEST_CASE("Jump from idle preserves horizontal velocity") {
    GameWorld world;
    world.setWorldSize(80, 24);
    Player* player = world.getPlayer();
    REQUIRE(player != nullptr);
    player->setGrounded(true);
    player->setVelocityX(50.0f);

    JumpCommand cmd;
    cmd.execute(*player);

    CHECK(dynamic_cast<JumpingState*>(player->getState()) != nullptr);
    CHECK(player->getVelocityX() == doctest::Approx(50.0f));
}

TEST_CASE("Direction change in running lets physics handle deceleration") {
    GameWorld world;
    world.setWorldSize(80, 24);
    Player* player = world.getPlayer();
    REQUIRE(player != nullptr);

    MoveRightCommand move_right;
    move_right.execute(*player);
    player->setVelocityX(Player::RUN_SPEED);

    MoveLeftCommand move_left;
    move_left.execute(*player);

    CHECK(player->getTargetVelocityX() == doctest::Approx(-Player::RUN_SPEED));
    CHECK(player->getVelocityX() == doctest::Approx(Player::RUN_SPEED));
}

TEST_CASE("JumpingState handles IdleCommand to zero target velocity") {
    GameWorld world;
    world.setWorldSize(80, 24);
    Player* player = world.getPlayer();
    REQUIRE(player != nullptr);
    player->setGrounded(true);

    MoveRightCommand move_cmd;
    move_cmd.execute(*player);

    JumpCommand jump_cmd;
    jump_cmd.execute(*player);

    CHECK(dynamic_cast<JumpingState*>(player->getState()) != nullptr);
    CHECK(player->getTargetVelocityX() == doctest::Approx(Player::RUN_SPEED));

    IdleCommand idle_cmd;
    idle_cmd.execute(*player);

    CHECK(dynamic_cast<JumpingState*>(player->getState()) != nullptr);
    CHECK(player->getTargetVelocityX() == doctest::Approx(0.0f));
}
