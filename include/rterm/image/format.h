#pragma once

#include <rterm/util/defs.h>
#include <array>

namespace rterm::image {

enum class Format {
    rgba32,
    bgra32,
};

template<Format format>
struct FormatTraits;

template<Format format>
struct Pixel {
    using Traits = FormatTraits<format>;
    using Channel = typename Traits::Channel;

    std::array<Channel, Traits::channels> channels;
};

template<>
struct FormatTraits<Format::rgba32> {
    using Channel = u8;
    static inline constexpr size_t channels = 4;
    static inline constexpr std::array<char, channels> rgbaOrder = {'r', 'g', 'b', 'a'};
};

template<>
struct FormatTraits<Format::bgra32> {
    using Channel = u8;
    static inline constexpr size_t channels = 4;
    static inline constexpr std::array<char, channels> rgbaOrder = {'b', 'g', 'r', 'a'};
};

template<Format from>
struct FormatConverter {
};

} // namespace rterm::image
