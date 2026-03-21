#pragma once

#include <string>
#include <vector>

namespace vertex {

struct Sprite {
    std::vector<std::string> lines;

    int width() const {
        if (lines.empty()) return 0;
        size_t max_width = 0;
        for (const auto& line : lines) {
            if (line.size() > max_width) max_width = line.size();
        }
        return static_cast<int>(max_width);
    }

    int height() const {
        return static_cast<int>(lines.size());
    }
};

}  // namespace vertex
