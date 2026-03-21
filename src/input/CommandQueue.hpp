#pragma once

#include <memory>
#include <queue>

namespace vertex {

class Command;

class CommandQueue {
public:
    void push(std::unique_ptr<Command> cmd);
    std::unique_ptr<Command> pop();
    bool isEmpty() const;

private:
    std::queue<std::unique_ptr<Command>> queue_;
};

}  // namespace vertex
