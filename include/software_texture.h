#pragma once


#include "sdl_ptr.h"
#include "rect.h"

#include <vector>
#include <SDL2/SDL.h>


namespace rterm {

class SoftwareTexture {
public:
    SoftwareTexture();
    SoftwareTexture(const SdlPtr<SDL_Surface>& surface);

    inline SDL_Surface* get() const {
        return surface_.get();
    }

    static constexpr uint32_t format = SDL_PIXELFORMAT_RGBA8888;

    operator bool() const {
        return !!surface_;
    }

private:
    SdlPtr<SDL_Surface> surface_;
};

} // namespace rterm
