#ifndef FONT_H
#define FONT_H

#include <cstddef>
#include <string>

#include <SDL2/SDL.h>

namespace term {
class Font
{
public:
    explicit Font(SDL_Renderer *ren = NULL, const std::string &pathToAsciiPng = "term.png");
    ~Font();
    Font& operator=(Font &&rhs);

    SDL_Texture * getTexture() const;
    SDL_Rect getRect(char c) const;

    size_t w() const;
    size_t h() const;
private:
    SDL_Texture *_font;
    SDL_Renderer *_ren;
};
}
#endif // FONT_H
