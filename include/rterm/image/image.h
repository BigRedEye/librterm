#pragma once

#include <rterm/image/format.h>
#include <rterm/color/color.h>
#include <rterm/util/span.h>

#include <vector>

namespace rterm::image {

template<Format format>
struct Pixel {
    using Channel = std::array<u8, bytesPerChannel>;
    std::array<Channel, FormatTraits<format>::channels> channels;
};

template<Format format, u32 bytesPerChannel>
class Image {
public:
    using Pixel = Pixel<format, bytesPerChannel>;

    const Pixel& operator()(size_t x, size_t y) const;
    Pixel& operator()(size_t x, size_t y);

    size_t width() const;
    size_t height() const;

    span<const u8> raw() const;

private:
    size_t at(size_t x, size_t y) const;

private:
    std::vector<Pixel> data_;
    size_t pixelWidth_;
};

template<Format format, u32 bytes>
const typename Image<format, bytes>::Pixel& Image<format, bytes>::operator()(size_t x, size_t y) const {
    return data_[at(x, y)];
}

template<Format format, u32 bytes>
typename Image<format, bytes>::Pixel& Image<format, bytes>::operator()(size_t x, size_t y) {
    return data_[at(x, y)];
}

template<Format format, u32 bytes>
size_t Image<format, bytes>::width() const {
    return pixelWidth_;
}

template<Format format, u32 bytes>
size_t Image<format, bytes>::height() const {
    return data_.size() / width();
}

template<Format format, u32 bytes>
size_t Image<format, bytes>::at(size_t x, size_t y) const {
    return y * width() + x;
}

template<Format format, u32 bytes>
span<const u8> Image<format, bytes>::raw() const {
    return {reinterpret_cast<const u8*>(data_.data()), data_.size() * sizeof(Pixel)};
}

} // namespace rterm::image
