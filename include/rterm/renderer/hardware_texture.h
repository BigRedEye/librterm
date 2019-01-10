#pragma once

#include "rterm/util/api.h"

namespace rterm {

template<Api::Type api>
class HardwareTextureBase;

template<Api::Type api>
class HardwareTexture;

} // namespace rterm

#include "gl_hardware_texture.tpp"
#include "sdl_hardware_texture.tpp"
