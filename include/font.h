#ifndef LIBTERM_FONT_H
#define LIBTERM_FONT_H

#include <cstddef>
#include <string>

#include <SDL2/SDL.h>

#include "color.h"

namespace term {

class Font
{
public:
    virtual void render(SDL_Renderer *p_ren, SDL_Rect dst, const char *str, Color fg, Color bg) {}

    virtual size_t w() const { return 1; }
    virtual size_t h() const { return 1; }
};
}
#endif // LIBTERM_FONT_H
