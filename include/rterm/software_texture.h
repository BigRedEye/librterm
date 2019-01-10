#pragma once

#include "error.h"
#include "rect.h"
#include "sdl_ptr.h"

#include <SDL.h>
#include <vector>

namespace rterm {

class SoftwareTexture {
public:
    SoftwareTexture() = default;
    SoftwareTexture(const SoftwareTexture& other);
    SoftwareTexture(SoftwareTexture&& moved) = default;

    SoftwareTexture(const std::string& path);
    SoftwareTexture(SDL_Surface* surface);
    SoftwareTexture(i32 w, i32 h);

    SoftwareTexture& operator=(const SoftwareTexture& other);
    SoftwareTexture& operator=(SoftwareTexture&& moved) = default;

    SDL_Surface* get() const {
        if (!surface_) {
            throw BadTexture();
        }
        return surface_.get();
    }

    ui8* data() const {
        /* cast to unsigned char is ok */
        return reinterpret_cast<ui8*>(get()->pixels);
    }

    SDL_Surface* operator->() const {
        return get();
    }

    operator bool() const {
        return surface_.get();
    }

    Vector<i32, 2> size() const {
        if (surface_) {
            return {surface_->w, surface_->h};
        }
        return {0, 0};
    }

    i32 w() const {
        return size()[0];
    }

    i32 h() const {
        return size()[1];
    }

    /**
     * @brief save image
     * @param file
     */
    void save(const std::string& file);

    static constexpr ui32 format = SDL_PIXELFORMAT_RGBA8888;

private:
    SdlHolder<SDL_Surface> surface_;
};

} // namespace rterm
