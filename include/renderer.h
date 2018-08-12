#pragma once


#include "api.h"
#include "sdl_ptr.h"
#include "texture_view.h"
#include "screen_view.h"


namespace rterm {

template<Api::Type api>
class Renderer;

} // namespace rterm

#include "sdl_renderer.tpp"
#include "gl_renderer.tpp"
