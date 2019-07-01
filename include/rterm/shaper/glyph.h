#pragma once

#include <rterm/util/defs.h>
#include <rterm/util/vector.h>

namespace rterm {

using FontID = u32;
using GlyphID = u32;

struct Glyph {
    FontID font;
    GlyphID id;
    vec2 offset;
    vec2 advance;
};

} // namespace rterm
