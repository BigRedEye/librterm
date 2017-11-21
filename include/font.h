#ifndef LIBTERM_FONT_H
#define LIBTERM_FONT_H

#include <cstddef>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "color.h"

namespace term {

class Font
{
public:
    Font();
    Font(const std::string &path, size_t sz);
    ~Font();
    Font& operator=(Font &&rhs);
    void destroyFont();

    void render(SDL_Renderer *p_ren, SDL_Rect dst, const char *str, Color fg, Color bg);

    size_t w() const;
    size_t h() const;
private:
    TTF_Font *p_font_;
};
}
#endif // LIBTERM_FONT_H
