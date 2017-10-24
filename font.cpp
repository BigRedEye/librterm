#include "font.h"
#include <SDL2/SDL_image.h>

namespace term {
Font::Font(SDL_Renderer *ren, const std::string &pathToAsciiPng)
    : _ren(ren), _font(NULL) {
    if (_ren)
        _font = IMG_LoadTexture(_ren, pathToAsciiPng.c_str());
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
    return *this;
}

size_t Font::w() const {
    return 12;
}

size_t Font::h() const {
    return 12;
}

SDL_Texture * Font::getTexture() const {
    return _font;
}

SDL_Rect Font::getRect(char c) const {
    return SDL_Rect{(c % 16) * w(), (c / 16) * h(), w(), h()};
}
}
