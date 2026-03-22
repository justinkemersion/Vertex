#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "game/GameWorld.hpp"
#include "entity/Player.hpp"
#include "physics/Vector2.hpp"
#include "game/Obstacle.hpp"

using namespace vertex;

TEST_CASE("PhysicsEngine: ground collision keeps player grounded") {
    GameWorld world;
    world.setWorldSize(80, 24);

    Player* player = world.getPlayer();
    REQUIRE(player != nullptr);

    const float ground_level = 24.0f - Player::SPRITE_HEIGHT - 2.0f;
    player->setPosition({5.0f, ground_level});
    player->setVelocityY(0.0f);
    player->setGrounded(true);

    world.getPhysics().update(world, 0.016f);

    CHECK(player->getPosition().y == doctest::Approx(ground_level));
    CHECK(player->getPosition().x == doctest::Approx(5.0f));
    CHECK(player->isGrounded() == true);
    CHECK(player->getVelocityY() == doctest::Approx(0.0f));
}

TEST_CASE("PhysicsEngine: ceiling collision clamps Y and zeroes upward velocity") {
    GameWorld world;
    world.setWorldSize(80, 24);

    Player* player = world.getPlayer();
    REQUIRE(player != nullptr);

    player->setPosition({40.0f, 1.0f});
    player->setVelocityY(-500.0f);
    player->setGrounded(false);

    world.getPhysics().update(world, 0.05f);

    CHECK(player->getPosition().y == doctest::Approx(0.0f));
    CHECK(player->getVelocityY() == doctest::Approx(0.0f));
}

TEST_CASE("PhysicsEngine: horizontal bounds clamp player") {
    GameWorld world;
    world.setWorldSize(80, 24);

    Player* player = world.getPlayer();
    REQUIRE(player != nullptr);

    const float max_x = static_cast<float>(world.getLevelWidth() - Player::SPRITE_WIDTH);
    player->setPosition({max_x + 50.0f, 19.0f});
    player->setVelocityX(100.0f);

    world.getPhysics().update(world, 0.016f);

    CHECK(player->getPosition().x == doctest::Approx(max_x));

    player->setPosition({-10.0f, 19.0f});
    player->setVelocityX(-100.0f);

    world.getPhysics().update(world, 0.016f);

    CHECK(player->getPosition().x == doctest::Approx(0.0f));
}

TEST_CASE("PhysicsEngine: gravity pulls player down") {
    GameWorld world;
    world.setWorldSize(80, 24);

    Player* player = world.getPlayer();
    REQUIRE(player != nullptr);

    player->setPosition({40.0f, 10.0f});
    player->setVelocityY(0.0f);
    player->setGrounded(false);

    world.getPhysics().update(world, 0.016f);

    CHECK(player->getPosition().y > 10.0f);
    CHECK(player->getVelocityY() > 0.0f);
}

TEST_CASE("PhysicsEngine: diagonal approach lands on obstacle, not blocked horizontally") {
    GameWorld world;
    world.setWorldSize(80, 24);

    Player* player = world.getPlayer();
    REQUIRE(player != nullptr);

    const auto& obstacles = world.getObstacles();
    REQUIRE(!obstacles.empty());
    const auto& obs = obstacles[0];

    const float pw = static_cast<float>(Player::SPRITE_WIDTH);
    const float ph = static_cast<float>(Player::SPRITE_HEIGHT);

    player->setPosition({obs.x - pw - 0.1f, obs.y - ph - 0.1f});
    player->setVelocityX(50.0f);
    player->setVelocityY(50.0f);
    player->setTargetVelocityX(50.0f);
    player->setGrounded(false);

    world.getPhysics().update(world, 0.016f);

    CHECK(player->getPosition().y == doctest::Approx(obs.y - ph));
    CHECK(player->isGrounded() == true);
    CHECK(player->getVelocityX() > 0.0f);
}

TEST_CASE("PhysicsEngine: horizontal collision blocks when grounded and approaching from side") {
    GameWorld world;
    world.setWorldSize(80, 24);

    Player* player = world.getPlayer();
    REQUIRE(player != nullptr);

    const auto& obs = world.getObstacles()[0];
    const float pw = static_cast<float>(Player::SPRITE_WIDTH);
    const float ground_level = static_cast<float>(world.getScreenHeight() - Player::SPRITE_HEIGHT - 2);

    player->setPosition({obs.x - pw - 0.1f, ground_level});
    player->setVelocityX(85.0f);
    player->setTargetVelocityX(85.0f);
    player->setVelocityY(0.0f);
    player->setGrounded(true);

    world.getPhysics().update(world, 0.016f);

    CHECK(player->getPosition().x <= obs.x - pw + 0.01f);
    CHECK(player->getVelocityX() == doctest::Approx(0.0f));
}

TEST_CASE("PhysicsEngine: horizontal collision does not stop jump while rising") {
    GameWorld world;
    world.setWorldSize(80, 24);

    Player* player = world.getPlayer();
    REQUIRE(player != nullptr);

    const auto& obs = world.getObstacles()[0];
    const float pw = static_cast<float>(Player::SPRITE_WIDTH);
    const float ground_level = static_cast<float>(world.getScreenHeight() - Player::SPRITE_HEIGHT - 2);

    player->setPosition({obs.x - pw - 0.1f, ground_level});
    player->setVelocityX(85.0f);
    player->setTargetVelocityX(85.0f);
    player->setVelocityY(-120.0f);
    player->setGrounded(false);

    world.getPhysics().update(world, 0.016f);

    CHECK(player->getVelocityX() == doctest::Approx(85.0f));
    CHECK(player->getPosition().x > obs.x - pw);
}
