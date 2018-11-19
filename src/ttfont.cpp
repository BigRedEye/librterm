#include "rterm/ttfont.h"
#include "rterm/error.h"
#include "rterm/logger.h"

#include <SDL_ttf.h>

namespace rterm {

TTFont::TTFont()
    : Font() {
}

TTFont::TTFont(const std::string& path, ui32 sz)
    : TTFont() {
    p_font_ = SdlHolder<TTF_Font>(TTF_OpenFont(path.c_str(), static_cast<i32>(sz)));
    if (!p_font_.get()) {
        throw BadFont();
    }
}

TTFont& TTFont::operator=(TTFont&& rhs) {
    p_font_ = std::move(rhs.p_font_);
    return *this;
}

ui32 TTFont::w() const {
    if (!p_font_.get()) {
        return 8;
    }
    static int w = -1, h = -1;
    if (w == -1) {
        TTF_SizeText(p_font_.get(), "@", &w, &h);
    }
    return w;
}

ui32 TTFont::h() const {
    if (!p_font_.get()) {
        return 8;
    }

    static int h = -1;
    if (h == -1) {
        h = TTF_FontHeight(p_font_.get());
    }
    return h;
}

SoftwareTexture TTFont::render(ch32 ch) {
    if (!p_font_.get()) {
        return SoftwareTexture(w(), h());
    }

    std::string str = utf32ToUtf8({ch});
    SDL_Surface* res = TTF_RenderUTF8_Blended(p_font_.get(), str.data(), SDL_Color{0xff, 0xff, 0xff, 0xff});
    if (!res) {
        throw BadFont();
    }
    return SoftwareTexture(res);
}

} // namespace rterm
