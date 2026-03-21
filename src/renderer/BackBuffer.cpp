#include "renderer/BackBuffer.hpp"
#include <algorithm>

namespace vertex {

BackBuffer::BackBuffer(int width, int height) : width_(width), height_(height) {
    buffer_.resize(static_cast<size_t>(height));
    for (auto& row : buffer_) {
        row.resize(static_cast<size_t>(width));
    }
}

void BackBuffer::clear(char fill) {
    for (auto& row : buffer_) {
        std::fill(row.begin(), row.end(), fill);
    }
}

void BackBuffer::put(int x, int y, char ch) {
    if (x >= 0 && x < width_ && y >= 0 && y < height_) {
        buffer_[static_cast<size_t>(y)][static_cast<size_t>(x)] = ch;
    }
}

void BackBuffer::put(int x, int y, const std::string& str) {
    for (size_t i = 0; i < str.size(); ++i) {
        put(x + static_cast<int>(i), y, str[i]);
    }
}

void BackBuffer::putSprite(int x, int y, const std::vector<std::string>& lines) {
    for (size_t row = 0; row < lines.size(); ++row) {
        const auto& line = lines[row];
        for (size_t col = 0; col < line.size(); ++col) {
            put(x + static_cast<int>(col), y + static_cast<int>(row), line[col]);
        }
    }
}

}  // namespace vertex
