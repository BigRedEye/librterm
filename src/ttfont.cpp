#include "error.h"
#include "logger.h"
#include "ttfont.h"

#include <SDL2/SDL_ttf.h>


namespace rterm {

TTFont::TTFont()
    : Font()
{
}

TTFont::TTFont(const std::string& path, size_t sz)
    : TTFont() {
    if (!TTF_WasInit()) {
        TTF_Init();
    }
    p_font_ = SdlHolder<TTF_Font>(TTF_OpenFont(path.c_str(), sz));
    if (!p_font_.get()) {
        Logger(Logger::ERROR) << TTF_GetError();
    }
}

TTFont& TTFont::operator=(TTFont&& rhs) {
    p_font_ = std::move(rhs.p_font_);
    return *this;
}

size_t TTFont::w() const {
    if (!p_font_.get()) {
        return 8;
    }
    static int w = -1, h = -1;
    if (w == -1) {
        TTF_SizeText(p_font_.get(), "@", &w, &h);
    }
    return w;
}

size_t TTFont::h() const {
    if (!p_font_.get()) {
        return 8;
    }

    static int h = -1;
    if (h == -1) {
        h = TTF_FontHeight(p_font_.get());
    }
    return h;
}

SoftwareTexture TTFont::render(char_t ch) {
    if (!p_font_.get()) {
        return SoftwareTexture(w(), h());
    }

    std::string str = Utf32ToUtf8({ch});
    SDL_Surface* res = TTF_RenderUTF8_Blended(
        p_font_.get(),
        str.data(),
        SDL_Color{0xff, 0xff, 0xff, 0xff}
    );
    if (!res) {
        throw BadFont();
    }
    return SoftwareTexture(res);

/*
    SDL_SetRenderDrawColor(p_ren, bg.r(), bg.g(), bg.b(), bg.a());
    SDL_RenderFillRect(p_ren, &dst);
    int h, w;
    SDL_QueryTexture(p_tex.get(), NULL, NULL, &w, &h);
    dst.h = h, dst.w = w;

    SDL_SetTextureColorMod(p_tex.get(), fg.r(), fg.g(), fg.b());
    SDL_RenderCopy(p_ren, p_tex.get(), NULL, &dst);
    SDL_SetTextureColorMod(p_tex.get(), 0xff, 0xff, 0xff);
*/
}

} // namespace rterm
