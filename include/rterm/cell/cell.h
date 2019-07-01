#pragma once

#include <rterm/util/defs.h>
#include <rterm/util/enum.h>
#include <rterm/color/color.h>

#include <string>

namespace rterm {

// https://vt100.net/docs/vt510-rm/SGR.html
// https://www.xfree86.org/current/ctlseqs.html
enum class Attributes : u32 {
    normal = 0,
    bold = 1 << 1,
    underline = 1 << 4,
    blink = 1 << 5,
    inverse = 1 << 7,
    hidden = 1 << 8,
};
RTERM_GENERATE_ENUM_BITWISE_OPERATORS(Attributes);

struct Cell {
    std::string text{}; ///< UTF-8 text; SSO
    Color background{};
    Color foreground{};
    Attributes attributes{};
};

static_assert(alignof(Cell) == 8);
static_assert(sizeof(Cell) == 48);

} // namespace rterm 
