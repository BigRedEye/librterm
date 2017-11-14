#include "font.h"
//#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

namespace term {
Font::Font()
    : p_ren_(NULL), p_font_(NULL) {
}

Font::Font(SDL_Renderer *ren, const std::string &path, size_t sz)
    : p_ren_(ren) {
    if (!TTF_WasInit())
        TTF_Init();
    p_font_ = TTF_OpenFont(path.c_str(), sz);
}

Font::~Font() {
    destroyFont();
}

void Font::destroyFont() {
    if (p_font_)
        TTF_CloseFont(p_font_);
    p_font_ = NULL;
}

Font& Font::operator=(Font &&rhs) {
    p_ren_ = rhs.p_ren_;
    rhs.p_ren_ = NULL;
    p_font_ = rhs.p_font_;
    rhs.p_font_ = NULL;
    return *this;
}

size_t Font::w() const {
    int w, h;
    TTF_SizeText(p_font_, "@", &w, &h);
    return w;
}

size_t Font::h() const {
    return TTF_FontHeight(p_font_);
}

SDL_Texture * Font::getTexture() const {
    return NULL;
}

SDL_Rect Font::getRect(char c) const {
    return SDL_Rect{static_cast<int>((c % 16) * w()),
                    static_cast<int>((c / 16) * h()),
                    static_cast<int>(w()),
                    static_cast<int>(h())};
}

void Font::setRenderer(SDL_Renderer *ren) {
    p_ren_ = ren;
}

void Font::render(SDL_Rect dst, const char *str, Color fg, Color bg) {
    SDL_Surface *psurf = TTF_RenderUTF8_Blended(p_font_,
                                                str,
                                                fg.toSDL_Color());
    SDL_SetRenderDrawColor(p_ren_, bg.r(), bg.g(), bg.b(), bg.a());
    SDL_RenderFillRect(p_ren_, &dst);
    SDL_Texture *tex = SDL_CreateTextureFromSurface(p_ren_, psurf);
    int h, w;
    SDL_QueryTexture(tex, NULL, NULL, &w, &h);
    dst.h = h, dst.w = w;
    SDL_RenderCopy(p_ren_, tex, NULL, &dst);
}
}
