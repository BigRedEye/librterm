#pragma once


#include "api.h"
#include "sdl_ptr.h"
#include "texture_view.h"
#include "screen_view.h"


namespace rterm {

template<Api::Type api>
class Renderer {
public:
    Renderer() = delete;
    Renderer(const Renderer& other) = delete;
    Renderer(Renderer&& other) = default;
    Renderer& operator=(const Renderer& other) = delete;
    Renderer& operator=(Renderer&& other) = default;

    void clear() const;
    void flush() const;
    void draw(const TextureView<api>& src, const ScreenView& dst) const;
};

} // namespace rterm

#include "sdl_renderer.h"
#include "gl_renderer.h"
