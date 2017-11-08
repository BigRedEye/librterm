#ifndef COLOR_H
#define COLOR_H

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
        return m_r;
    }
    uint8_t& g() {
        return m_g;
    }
    uint8_t& b() {
        return m_b;
    }
    uint8_t& a() {
        return m_a;
    }

    uint8_t r() const {
        return m_r;
    }
    uint8_t g() const {
        return m_g;
    }
    uint8_t b() const {
        return m_b;
    }
    uint8_t a() const {
        return m_a;
    }
private:
    uint8_t m_r, m_g, m_b, m_a;
};
}


#endif // COLOR_H
