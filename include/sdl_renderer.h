#pragma once

#include "renderer.h"

namespace rterm {

template<>
class Renderer<Api::SDL> {
public:
    explicit Renderer(SDL_Renderer* renderer = nullptr)
        : renderer_(renderer)
    {
    }

    inline SDL_Renderer* get() const {
        return renderer_.get();
    }
    
    void clear() const;
    void flush() const;
    void draw(const TextureView<Api::SDL>& src, const ScreenView& dst);

private:
    SdlPtr<SDL_Renderer> renderer_;
    /* HardwareTexture<Api::SDL> buffer_; */
};


} // namespace rterm
