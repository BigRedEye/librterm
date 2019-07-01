#include <rterm/buffer/buffer.h>

namespace rterm {

void Buffer::fillArgs(widthTag tag) {
    width_ = tag;
}

void Buffer::fillArgs(heightTag tag) {
    height_ = tag;
}

void Buffer::resize(size_t width, size_t height) { 
    if (height != dynamic) {
        rows_.resize(height);
    }

    if (width != dynamic) {
        for (auto&& row : rows_) {
            row.resize(width);
        }
    }
}

} // namespace rterm
