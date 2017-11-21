#include "font.h"
//#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

namespace term {
Font::Font()
    : p_font_(NULL) {
}

Font::Font(const std::string &path, size_t sz) {
    if (!TTF_WasInit())
        TTF_Init();
    p_font_ = TTF_OpenFont(path.c_str(), sz);
    if (!p_font_)
        SDL_Log(TTF_GetError());
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
    p_font_ = rhs.p_font_;
    rhs.p_font_ = NULL;
    return *this;
}

size_t Font::w() const {
    if (!p_font_)
        return 1;

    int w, h;
    TTF_SizeText(p_font_, "@", &w, &h);
    return w;
}

size_t Font::h() const {
    if (!p_font_)
        return 1;

    return TTF_FontHeight(p_font_);
}

void Font::render(SDL_Renderer *p_ren, SDL_Rect dst, const char *str, Color fg, Color bg) {
    if (!p_font_)
        return;
    SDL_Surface *p_surf = TTF_RenderUTF8_Blended(p_font_,
                                            str,
                                            fg.toSDL_Color());
    SDL_SetRenderDrawColor(p_ren, bg.r(), bg.g(), bg.b(), bg.a());
    SDL_RenderFillRect(p_ren, &dst);
    SDL_Texture *tex = SDL_CreateTextureFromSurface(p_ren, p_surf);
    int h, w;
    SDL_QueryTexture(tex, NULL, NULL, &w, &h);
    dst.h = h, dst.w = w;
    SDL_RenderCopy(p_ren, tex, NULL, &dst);
}
}
