#include "ttfont.h"
#include <SDL2/SDL_ttf.h>

namespace term {
TTFont::TTFont()
    : Font() {
}

TTFont::TTFont(const std::string &path, size_t sz)
    : TTFont() {
    if (!TTF_WasInit())
        TTF_Init();
    p_font_ = SDL_Ptr<TTF_Font>(TTF_OpenFont(path.c_str(), sz));
    if (!p_font_.get())
        SDL_Log(TTF_GetError());
}

TTFont& TTFont::operator=(TTFont &&rhs) {
    p_font_ = std::move(rhs.p_font_);
    return *this;
}

size_t TTFont::w() const {
    if (!p_font_.get())
        return 1;

    int w, h;
    TTF_SizeText(p_font_.get(), "@", &w, &h);
    return w;
}

size_t TTFont::h() const {
    if (!p_font_.get())
        return 1;

    return TTF_FontHeight(p_font_.get());
}

void TTFont::render(SDL_Renderer *p_ren, SDL_Rect dst, const char *str, Color fg, Color bg) {
    if (!p_font_.get())
        return;
    SDL_Ptr<SDL_Surface> p_surf(TTF_RenderUTF8_Blended(p_font_.get(),
                                            str,
                                            fg.toSDL_Color()));
    SDL_SetRenderDrawColor(p_ren, bg.r(), bg.g(), bg.b(), bg.a());
    SDL_RenderFillRect(p_ren, &dst);
    SDL_Ptr<SDL_Texture> tex(SDL_CreateTextureFromSurface(p_ren, p_surf.get()));
    int h, w;
    SDL_QueryTexture(tex.get(), NULL, NULL, &w, &h);
    dst.h = h, dst.w = w;
    SDL_RenderCopy(p_ren, tex.get(), NULL, &dst);
}
}
