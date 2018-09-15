/**
 * @file
 */

#pragma once

/// @cond INTERNAL

#include "char.h"
#include "font.h"
#include "sdl_ptr.h"

#include <SDL_ttf.h>

namespace rterm {

/**
 * @brief The TTFont class
 *
 * This class represents true type fonts
 *
 * @see TileFont for tilemap fonts
 */
class TTFont : public Font {
public:
    /**
     * @brief Constructs empty TTFont
     */
    TTFont();

    ~TTFont() override = default;

    /**
     * @brief Constructs TTFont from the given .ttf file
     * @param path path to .ttf file
     * @param sz point size
     */
    TTFont(const std::string& path, ui32 sz);

    /**
     * @brief Move operator=
     * @param rhs rvalue reference to another TileFont
     * @return reference to this object
     */
    TTFont& operator=(TTFont&& rhs);

    /**
     * @brief Render an utf-8 string
     * @param p_ren renderer for current render target
     * @param dst rect on renderer target where the string should be rendered
     * @param str string that should be rendered
     * @param fg foreground color
     * @param bg background color
     */
    virtual SoftwareTexture render(char_t ch) override;

    /**
     * @brief Font width
     * @return width of one character
     */
    virtual ui32 w() const override;

    /**
     * @brief Font height
     * @return height of one character
     */
    virtual ui32 h() const override;

private:
    SdlHolder<TTF_Font> p_font_; ///< real font
};

} // namespace rterm

/// @endcond
