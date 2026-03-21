#include "input/CommandQueue.hpp"
#include "input/Command.hpp"

namespace vertex {

void CommandQueue::push(std::unique_ptr<Command> cmd) {
    if (cmd) {
        queue_.push(std::move(cmd));
    }
}

std::unique_ptr<Command> CommandQueue::pop() {
    if (queue_.empty()) return nullptr;
    auto cmd = std::move(queue_.front());
    queue_.pop();
    return cmd;
}

bool CommandQueue::isEmpty() const {
    return queue_.empty();
}

}  // namespace vertex
