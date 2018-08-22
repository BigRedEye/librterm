/**
 * @file
 */

#pragma once

/// @cond INTERNAL

#include "char.h"
#include "font.h"
#include "sdl_ptr.h"

namespace rterm {

/**
 * @brief The TileFont class
 *
 * This class represents tilemap fonts
 *
 * @see TTFont for true type fonts
 */
class TileFont : public Font {
public:
    /**
     * @brief Constructs empty TileFont
     */
    TileFont();

    ~TileFont() override = default;

    /**
     * @brief Constructs TileFont from given tilemap
     * @param path path to the tilemap (image file)
     * @param _w width of a tile in pixels
     * @param _h height of a tile in pixels
     */
    TileFont(const std::string& path, size_t _w, size_t _h);

    /**
     * @brief Move operator=
     * @param rhs rvalue reference to another TileFont
     * @return reference to this object
     */
    TileFont& operator=(TileFont&& rhs);

    virtual SoftwareTexture render(char_t ch) override;

    /**
     * @brief Font width
     * @return width of a tile
     */
    virtual size_t w() const override;

    /**
     * @brief Font height
     * @return height of a tile
     */
    virtual size_t h() const override;

private:
    /**
     * @brief Get tile on th tilemap
     * @param c UTF8 character
     * @return SDL_Rect of tile on the tilemap
     */
    SDL_Rect getRect(char_t c) const;

    SdlHolder<SDL_Surface> p_tilemap_;        ///< tilemap surface
    SdlHolder<SDL_Texture> p_tilemapTexture_; ///< cached tilemap texture
    size_t w_,                                ///< tile width
        h_;                                   ///< tile height
};

} // namespace rterm

/// @endcond
