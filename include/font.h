/**
 * @file
 * @brief This file contains abstract font clas
 */

#pragma once

/// @cond INTERNAL

#include "char.h"
#include "color.h"

#include "software_texture.h"

#include <cstddef>
#include <string>

namespace rterm {

/**
 * @brief The Font class
 *
 * Abstract font rasterizer
 */
class Font {
public:
    virtual ~Font() = default;

    /**
     * @brief generate SoftwareTexture for character
     * @param ch utf-32 character
     * @return generated SoftwareTexture
     * @note this function doesn't provide any caching
     */
    virtual SoftwareTexture render(char_t ch) = 0;

    /**
     * @brief font width
     * @return width of a single character
     */
    virtual ui32 w() const = 0;

    /**
     * @brief font height
     * @return height of a single character
     */
    virtual ui32 h() const = 0;
};

} // namespace rterm

/// @endcond
