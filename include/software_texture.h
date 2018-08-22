#pragma once

#include "rect.h"
#include "sdl_ptr.h"

#include <SDL2/SDL.h>
#include <vector>

namespace rterm {

class SoftwareTexture {
public:
    SoftwareTexture() = default;
    SoftwareTexture(const SoftwareTexture& other);
    SoftwareTexture(SoftwareTexture&& moved) = default;
    SoftwareTexture(SDL_Surface* surface);
    SoftwareTexture(i32 w, i32 h);

    SoftwareTexture& operator=(const SoftwareTexture& other);
    SoftwareTexture& operator=(SoftwareTexture&& moved) = default;

    inline SDL_Surface* get() const {
        return surface_.get();
    }

    inline SDL_Surface* operator->() const {
        return surface_.get();
    }

    inline operator bool() const {
        return surface_.operator bool();
    }

    inline Vector<i32, 2> size() const {
        if (surface_) {
            return {surface_->w, surface_->h};
        }
        return {0, 0};
    }

    inline i32 w() const {
        return size()[0];
    }

    inline i32 h() const {
        return size()[1];
    }

    static constexpr ui32 format = SDL_PIXELFORMAT_RGBA8888;

private:
    SdlHolder<SDL_Surface> surface_;
};

} // namespace rterm
