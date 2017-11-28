/**
 * @file
 * @brief This file contains abstract font clas
 */

#ifndef RTERM_FONT_H
#define RTERM_FONT_H

/// @cond INTERNAL

#include <cstddef>
#include <string>

#include <SDL2/SDL.h>

#include "color.h"
#include "char.h"

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
    virtual void render(SDL_Renderer *p_ren, SDL_Rect dst, char_t str, Color fg, Color bg) {}

    /**
     * @brief font width
     * @return width of a single character
     */
    virtual size_t w() const { return 1; }

    /**
     * @brief font height
     * @return height of a single character
     */
    virtual size_t h() const { return 1; }
};
}

/// @endcond

#endif // RTERM_FONT_H
