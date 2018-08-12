#pragma once


#include "api.h"


namespace rterm {

template<Api::Type api>
class HardwareTextureBase;

template<Api::Type api>
class HardwareTexture;

} // namespace rterm

#include "sdl_hardware_texture.tpp"
#include "gl_hardware_texture.tpp"
