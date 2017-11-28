/**
 * @file
 */

#ifndef RTERM_TTFONT_H
#define RTERM_TTFONT_H

/// @cond INTERNAL

#include <unordered_map>

#include <SDL2/SDL_ttf.h>
#include <vector>

#include "font.h"
#include "sdl_ptr.h"
#include "char.h"
#include <queue>

namespace rterm {

template<typename _Key,
         typename _Val,
         typename _SparceContainer = std::unordered_map<_Key, _Val>,
         typename _DenseContainer = std::vector<std::pair<_Val, bool>>>
class FastCharUnorderedMap {
public:
    FastCharUnorderedMap(_Key maxKey = defaultMaxKey);

    bool get(_Key key, _Val &val);
    void set(_Key key, _Val val);
private:
    static constexpr _Key defaultMaxKey = _Key(0xff);
    
    _Key maxKey_;

    _SparceContainer mappedKeys_;
    _DenseContainer smallKeys_;
};

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

    /**
     * @brief Constructs TTFont from the given .ttf file
     * @param path path to .ttf file
     * @param sz point size
     */
    TTFont(const std::string &path, size_t sz);
    
    /**
     * @brief Move operator=
     * @param rhs rvalue reference to another TileFont
     * @return reference to this object
     */
    TTFont& operator=(TTFont &&rhs);

    /**
     * @brief Render a string of UTF8 bytes
     * @param p_ren renderer for current render target
     * @param dst rect on renderer target where the string should be rendered
     * @param str string that should be rendered
     * @param fg foreground color
     * @param bg background color
     */
    virtual void render(SDL_Renderer *p_ren, SDL_Rect dst, char_t ch, Color fg, Color bg) override;

    /**
     * @brief Font width
     * @return width of one character
     */
    virtual size_t w() const override;

    /**
     * @brief Font height
     * @return height of one character
     */
    virtual size_t h() const override;

private:
    SDL_Ptr<TTF_Font> p_font_; ///< real font
    
    FastCharUnorderedMap<char_t, SDL_SharedPtr<SDL_Texture>> cache_; ///< cached rendered symbols
};

}

/// @endcond

#endif // RTERM_TTFONT_H

