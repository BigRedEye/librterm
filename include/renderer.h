#pragma once

#include "api.h"
#include "screen_view.h"
#include "sdl_ptr.h"
#include "texture_view.h"

namespace rterm {

template<Api::Type api>
class Renderer;

} // namespace rterm

#include "gl_renderer.tpp"
#include "sdl_renderer.tpp"
