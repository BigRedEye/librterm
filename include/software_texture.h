#pragma once


#include "sdl_ptr.h"
#include "rect.h"

#include <vector>
#include <SDL2/SDL.h>


namespace rterm {

class SoftwareTexture {
public:
    SoftwareTexture() = default;
    SoftwareTexture(const SoftwareTexture& other);
    SoftwareTexture(SoftwareTexture&& moved) = default;
    SoftwareTexture(SDL_Surface* surface);
    SoftwareTexture(int w, int h);

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

    inline Vector<int, 2> size() const {
        if (surface_) {
            return {surface_->w, surface_->h};
        }
        return {0, 0};
    }

    inline int w() const {
        return size()[0];
    }

    inline int h() const {
        return size()[1];
    }

    static constexpr uint32_t format = SDL_PIXELFORMAT_RGBA8888;

private:
    SdlHolder<SDL_Surface> surface_;
};

} // namespace rterm
