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
    explicit Font(SDL_Renderer *ren = NULL, const std::string &pathToAsciiPng = "term.png");
    ~Font();
    Font& operator=(Font &&rhs);

    SDL_Texture * getTexture() const;
    SDL_Rect getRect(char c) const;
    void setRenderer(SDL_Renderer *ren);

    void render(SDL_Rect dst, const char *str, Color fg, Color bg);

    size_t w() const;
    size_t h() const;
private:
    SDL_Texture *_font;
    TTF_Font *font;
    SDL_Renderer *_ren;
};
}
#endif // FONT_H
