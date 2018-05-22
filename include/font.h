/**
 * @file
 * @brief This file contains abstract font clas
 */

#pragma once


/// @cond INTERNAL


#include "color.h"
#include "char.h"

#include <SDL2/SDL.h>

#include <cstddef>
#include <string>


namespace rterm {

/**
 * @brief The Font class
 *
 * Abstract font class that contains font data and can render string with SDL_Renderer
 */
class Font
{
public:
    virtual ~Font() = default;

    /**
     * @brief render a string of UTF8 bytes
     * @param p_ren renderer for current render target
     * @param dst rect on renderer target where the string should be rendered
     * @param str string that should be rendered
     * @param fg foreground color
     * @param bg background color
     */
    virtual void render(SDL_Renderer* p_ren, SDL_Rect dst, char_t str, Color fg, Color bg) = 0;

    /**
     * @brief font width
     * @return width of a single character
     */
    virtual size_t w() const = 0;

    /**
     * @brief font height
     * @return height of a single character
     */
    virtual size_t h() const = 0;
};

} // namespace rterm

/// @endcond
