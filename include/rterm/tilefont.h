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
    TileFont(const std::string& path, ui32 _w, ui32 _h);

    /**
     * @brief Move operator=
     * @param rhs rvalue reference to another TileFont
     * @return reference to this object
     */
    TileFont& operator=(TileFont&& rhs);

    virtual SoftwareTexture render(ch32 ch) override;

    /**
     * @brief Font width
     * @return width of a tile
     */
    virtual ui32 w() const override;

    /**
     * @brief Font height
     * @return height of a tile
     */
    virtual ui32 h() const override;

private:
    /**
     * @brief Get tile on th tilemap
     * @param c UTF8 character
     * @return SDL_Rect of tile on the tilemap
     */
    SDL_Rect getRect(ch32 c) const;

    SdlHolder<SDL_Surface> p_tilemap_;        ///< tilemap surface
    SdlHolder<SDL_Texture> p_tilemapTexture_; ///< cached tilemap texture
    ui32 w_;                                  ///< tile width
    ui32 h_;                                  ///< tile height
};

} // namespace rterm

/// @endcond
