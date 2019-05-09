#pragma once

#include <rterm/util/defs.h>
#include <rterm/util/enum.h>

namespace rterm {

enum class Api : u8 {
    OpenGL = 1,
    Vulkan = 2,
    best = OpenGL | Vulkan,
};
RTERM_GENERATE_ENUM_BITWISE_OPERATORS(Api);

} // namespace rterm
