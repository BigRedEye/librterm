#pragma once

#include <rterm/util/defs.h>
#include <rterm/util/enum.h>

namespace rterm {

enum class Api : u8 {
    OpenGL = 1,
    Vulkan = 2,
};
GENERATE_ENUM_BITWISE_OPERATORS(Api);

static_assert((Api::OpenGL ^ Api::OpenGL | Api::Vulkan) == Api::Vulkan);

} // namespace rterm
