#pragma once

#include <rterm/shaper/glyph.h>

#include <cstddef>
#include <vector>

namespace rterm {

struct Cluster {
    size_t offset;
    std::vector<Glyph> glyphs;
};

} // namespace rterm
