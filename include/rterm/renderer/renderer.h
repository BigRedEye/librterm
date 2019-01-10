#pragma once

#include "rterm/util/api.h"
#include "rterm/util/sdl/sdl_ptr.h"
#include "rterm/renderer/screen_view.h"
#include "rterm/renderer/texture_view.h"

namespace rterm {

template<Api::Type api>
class Renderer;

} // namespace rterm

#include "gl_renderer.tpp"
#include "sdl_renderer.tpp"
