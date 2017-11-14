#ifndef FONT_H
#define FONT_H

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
    Font(SDL_Renderer *ren, const std::string &path, size_t sz);
    ~Font();
    Font& operator=(Font &&rhs);
    void destroyFont();

    SDL_Texture * getTexture() const;
    SDL_Rect getRect(char c) const;
    void setRenderer(SDL_Renderer *ren);

    void render(SDL_Rect dst, const char *str, Color fg, Color bg);

    size_t w() const;
    size_t h() const;
private:
    TTF_Font *p_font_;
    SDL_Renderer *p_ren_;
};
}
#endif // FONT_H