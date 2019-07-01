#pragma once

#include <rterm/cell/cell.h>
#include <rterm/util/defs.h>
#include <rterm/util/vector.h>
#include <rterm/util/named_args.h>
#include <limits>
#include <vector>

namespace rterm {

inline constexpr size_t dynamic = std::numeric_limits<size_t>::max();

RTERM_DECLARE_NAMED_ARGUMENT(size_t, width);
RTERM_DECLARE_NAMED_ARGUMENT(size_t, height);

class Buffer {
public:
    template<typename ...Args>
    Buffer(Args&& ...args) {
        ((fillArgs(std::forward<Args>(args)), 0) + ... + 0);
        resize(width_, height_);
    }

    Row& row(size_t y);
    const Row& row(size_t y) const;

    Cell& operator()(size_t x, size_t y);
    const Cell& operator()(size_t x, size_t y) const;
    Cell& operator[](uvec2 pos);
    const Cell& operator[](uvec2 pos) const;

    void resize(size_t width, size_t height);

    Vector<size_t, 2> size() const;
    size_t width() const;
    size_t height() const;

private:
    void fillArgs(widthTag tag);
    void fillArgs(heightTag tag);

    template<typename T, bool False = false>
    void fillArgs(T) {
        static_assert(False, "Invalid argument");
    }

private:
    size_t width_;
    size_t height_;
    std::vector<Row> rows_;
};

} // namespace rterm
