#pragma once

#include <memory>

namespace vertex {

class BackBuffer;
class GameWorld;

class Renderer {
public:
    Renderer();
    ~Renderer();

    void clear();
    void draw(const GameWorld& world);
    void present();

private:
    std::unique_ptr<BackBuffer> buffer_;
    int width_{0};
    int height_{0};
};

}  // namespace vertex
