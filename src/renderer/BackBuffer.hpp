#pragma once

#include <string>
#include <vector>

namespace vertex {

class BackBuffer {
public:
    BackBuffer(int width, int height);

    void clear(char fill = ' ');
    void put(int x, int y, char ch);
    void put(int x, int y, const std::string& str);
    void putSprite(int x, int y, const std::vector<std::string>& lines);

    int width() const { return width_; }
    int height() const { return height_; }
    const std::vector<std::string>& data() const { return buffer_; }

private:
    int width_;
    int height_;
    std::vector<std::string> buffer_;
};

}  // namespace vertex
