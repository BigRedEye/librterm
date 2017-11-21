#ifndef LIBTERM_COLOR_H
#define LIBTERM_COLOR_H

#include <cstdint>
#include <SDL2/SDL_pixels.h>

namespace term {
class Color {
public:
    explicit Color(uint8_t _r = 0,
                   uint8_t _g = 0,
                   uint8_t _b = 0,
                   uint8_t _a = 0);

    SDL_Color toSDL_Color() const {
        return SDL_Color{r(), g(), b(), a()};
    }

    uint8_t& r() {
        return r_;
    }
    uint8_t& g() {
        return g_;
    }
    uint8_t& b() {
        return b_;
    }
    uint8_t& a() {
        return a_;
    }

    uint8_t r() const {
        return r_;
    }
    uint8_t g() const {
        return g_;
    }
    uint8_t b() const {
        return b_;
    }
    uint8_t a() const {
        return a_;
    }
private:
    uint8_t r_, g_, b_, a_;
};
}


#endif // LIBTERM_COLOR_H
