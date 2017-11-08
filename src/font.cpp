#include "font.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

namespace term {
Font::Font(SDL_Renderer *ren, const std::string &pathToAsciiPng)
    : _ren(ren), _font(NULL) {
    font = TTF_OpenFont("/usr/share/fonts/TTF/DejaVuSansMono.ttf", 3); /* TTF_OpenFont("/home/sergey/Downloads/latest/terminus-ttf-4.46.0/TerminusTTF-4.46.0.ttf",
                        18); */
    std::cerr << TTF_GetError() << std::endl;
}

Font::~Font() {
    if (_font)
        SDL_DestroyTexture(_font);
}

Font& Font::operator=(Font &&rhs) {
    if (_font)
        SDL_DestroyTexture(_font);
    _font = rhs._font;
    _ren = rhs._ren;
    rhs._font = NULL;
    rhs._ren = NULL;
    font = rhs.font;
    rhs.font = NULL;
    return *this;
}

size_t Font::w() const {
    int w, h;
    TTF_SizeText(font, "@", &w, &h);
    return w;
}

size_t Font::h() const {
    return TTF_FontHeight(font);
}

SDL_Texture * Font::getTexture() const {
    return NULL;
}

SDL_Rect Font::getRect(char c) const {
    return SDL_Rect{(c % 16) * w(), (c / 16) * h(), w(), h()};
}

void Font::setRenderer(SDL_Renderer *ren) {
    _ren = ren;
}

void Font::render(SDL_Rect dst, const char *str, Color fg, Color bg) {
    SDL_Surface *psurf = TTF_RenderUTF8_Blended(font,
                                                str,
                                                fg.toSDL_Color());
    // SDL_Rect src = getRect(get(x, y));
    SDL_SetRenderDrawColor(_ren, bg.r(), bg.g(), bg.b(), bg.a());
    SDL_RenderFillRect(_ren, &dst);
    SDL_Texture *tex = SDL_CreateTextureFromSurface(_ren, psurf);
    int h, w;
    SDL_QueryTexture(tex, NULL, NULL, &w, &h);
    dst.h = h, dst.w = w;
    SDL_RenderCopy(_ren, tex, NULL, &dst);
}
}
