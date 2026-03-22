#pragma once

namespace vertex {

struct Obstacle {
    float x{0.0f};
    float y{0.0f};
    int width{1};
    int height{1};

    float right() const { return x + static_cast<float>(width); }
    float bottom() const { return y + static_cast<float>(height); }
};

}  // namespace vertex
