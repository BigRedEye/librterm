/// @cond INTERNAL

#ifndef RTERM_TTFONT_H
#define RTERM_TTFONT_H

#include <SDL2/SDL_ttf.h>

#include "font.h"
#include "sdl_ptr.h"

namespace rterm {

class TTFont : public Font {
public:
    TTFont();
    TTFont(const std::string &path, size_t sz);

    TTFont& operator=(TTFont &&rhs);

    virtual void render(SDL_Renderer *p_ren, SDL_Rect dst, const char *str, Color fg, Color bg) override;

    virtual size_t w() const override;
    virtual size_t h() const override;

private:
    SDL_Ptr<TTF_Font> p_font_;
};
}

#endif // RTERM_TTFONT_H

/// @endcond INTERNAL
