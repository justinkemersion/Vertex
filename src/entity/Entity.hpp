#pragma once

#include "physics/Vector2.hpp"
#include "renderer/Sprite.hpp"
#include <memory>

namespace vertex {

class Entity {
public:
    virtual ~Entity() = default;

    Vector2 getPosition() const { return position_; }
    void setPosition(Vector2 pos) { position_ = pos; }

    virtual const Sprite& getSprite() const = 0;

protected:
    Vector2 position_;
};

}  // namespace vertex
