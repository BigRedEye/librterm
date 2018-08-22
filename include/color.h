/**
 * @file
 * @brief This file contains custom class for storing colors
 * @author BigRedEye
 */

#pragma once

#include "util.h"

#include <SDL2/SDL_pixels.h>

#include <cstdint>

namespace rterm {

/**
 * @brief The Color class
 *
 * Object of this class is a color in format RGBA8888 (8 bits for each channel)
 */
class alignas(ui32) Color {
public:
    /**
     * @brief Constuctor for Color object
     * @param _r red channel
     * @param _g green channel
     * @param _b blue channel
     * @param _a alpha channel
     */
    Color(ui8 _r = 0, ui8 _g = 0, ui8 _b = 0, ui8 _a = 0);

    bool operator==(const Color& other) const;
    bool operator!=(const Color& other) const;

    /**
     * @brief convert Color to SDL_Color
     * @return SDL_Color represents the same color as this object
     */
    inline SDL_Color toSdlColor() const {
        return SDL_Color{r(), g(), b(), a()};
    }

    /**
     * @brief set red component
     */
    inline void r(ui8 _r) {
        r_ = _r;
    }

    /**
     * @brief get reference to green component
     */
    inline void g(ui8 _g) {
        g_ = _g;
    }

    /**
     * @brief get reference to blue component
     */
    inline void b(ui8 _b) {
        b_ = _b;
    }

    /**
     * @brief get reference to alpha component
     */
    inline void a(ui8 _a) {
        a_ = _a;
    }

    /**
     * @brief get red component
     * @return red component
     */
    ui8 r() const {
        return r_;
    }

    /**
     * @brief get green component
     * @return green component
     */
    ui8 g() const {
        return g_;
    }

    /**
     * @brief get blue component
     * @return blue component
     */
    ui8 b() const {
        return b_;
    }

    /**
     * @brief get alpha component
     * @return alpha component
     */
    ui8 a() const {
        return a_;
    }

    ui32 rgba() const;

    static const Color Black;
    static const Color Red;
    static const Color Green;
    static const Color Yellow;
    static const Color Blue;
    static const Color Magenta;
    static const Color Cyan;
    static const Color White;

private:
    ui8 r_; ///< red channel
    ui8 g_; ///< green channel
    ui8 b_; ///< blue channel
    ui8 a_; ///< alpha channel
};

} // namespace rterm
