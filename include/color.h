/**
 * @file
 * @brief This file contains custom class for storing colors
 * @author BigRedEye
 */

#pragma once

#include <cstdint>
#include <SDL2/SDL_pixels.h>

namespace rterm {

/**
 * @brief The Color class
 * 
 * Object of this class is a color in format RGBA8888 (8 bits for each channel)
 */
class Color {
public:
    /**
     * @brief Constuctor for Color object
     * @param _r red channel
     * @param _g green channel
     * @param _b blue channel
     * @param _a alpha channel
     */
    explicit Color(uint8_t _r = 0,
                   uint8_t _g = 0,
                   uint8_t _b = 0,
                   uint8_t _a = 0);

    bool operator==(const Color &other) const;
    bool operator!=(const Color &other) const;

    /**
     * @brief convert Color to SDL_Color
     * @return SDL_Color represents the same color as this object
     */
    SDL_Color toSdlColor() const {
        return SDL_Color{r(), g(), b(), a()};
    }

    /**
     * @brief get reference to red component
     * @return reference to red component
     */
    uint8_t& r() {
        return r_;
    }
    
    /**
     * @brief get reference to green component
     * @return reference to green component
     */
    uint8_t& g() {
        return g_;
    }
    
    /**
     * @brief get reference to blue component
     * @return reference to blue component
     */
    uint8_t& b() {
        return b_;
    }
    
    /**
     * @brief get reference to alpha component
     * @return reference to alpha component
     */
    uint8_t& a() {
        return a_;
    }
    
    /**
     * @brief get red component
     * @return red component
     */
    uint8_t r() const {
        return r_;
    }
    
    /**
     * @brief get green component
     * @return green component
     */
    uint8_t g() const {
        return g_;
    }
    
    /**
     * @brief get blue component
     * @return blue component
     */
    uint8_t b() const {
        return b_;
    }
    
    /**
     * @brief get alpha component
     * @return alpha component
     */
    uint8_t a() const {
        return a_;
    }

private:
    uint8_t r_; ///< red channel 
    uint8_t g_; ///< green channel
    uint8_t b_; ///< blue channel
    uint8_t a_; ///< alpha channel
};
}
