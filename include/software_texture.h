#pragma once


#include "sdl_ptr.h"
#include "rect.h"

#include <vector>
#include <SDL2/SDL.h>


namespace rterm {

template<typename ValueType = uint8_t>
struct PixelRgba {
    static constexpr int format = 0;

    ValueType data[4];

    inline ValueType& r() {
        return data[0];
    }

    inline ValueType& g() {
        return data[1];
    }

    inline ValueType& b() {
        return data[2];
    }

    inline ValueType& a() {
        return data[3];
    }
};

template<>
class PixelRgba<uint8_t> {
    static constexpr int format = SDL_PIXELFORMAT_RGBA8888;    
};

using PixelRgba32 = PixelRgba<uint8_t>;

template<typename Pixel = PixelRgba32>
class SoftwareTexture {
public:
    SoftwareTexture();
    SoftwareTexture(SDL_Surface* surface);
    SoftwareTexture(const SdlPtr<SDL_Surface>& ptr);

private:
    size_t w_;
    size_t h_;
    std::vector<Pixel> pixels_;
};

} // namespace rterm
