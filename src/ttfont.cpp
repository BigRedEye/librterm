#include "ttfont.h"
#include <SDL2/SDL_ttf.h>

namespace rterm {
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

void TTFont::render(SDL_Renderer *p_ren, SDL_Rect dst, char_t ch, Color fg, Color bg) {
    if (!p_font_.get())
        return;

    SDL_SharedPtr<SDL_Texture> p_tex;
    if (!cache_.get(ch, p_tex)) {
        std::string str = UTF8CharToBytes(ch);
        SDL_Ptr<SDL_Surface> p_surf(TTF_RenderUTF8_Blended(p_font_.get(),
                                    str.c_str(),
                                    SDL_Color{255, 255, 255}));
        p_tex = make_SDL_SharedPtr(SDL_CreateTextureFromSurface(p_ren, p_surf.get()));
        cache_.set(ch, p_tex);
    }

    SDL_SetRenderDrawColor(p_ren, bg.r(), bg.g(), bg.b(), bg.a());
    SDL_RenderFillRect(p_ren, &dst);
    int h, w;
    SDL_QueryTexture(p_tex.get(), NULL, NULL, &w, &h);
    dst.h = h, dst.w = w;

    SDL_SetTextureColorMod(p_tex.get(), fg.r(), fg.g(), fg.b());
    SDL_RenderCopy(p_ren, p_tex.get(), NULL, &dst);
    SDL_SetTextureColorMod(p_tex.get(), 0xff, 0xff, 0xff);
}

template<typename _Key,
         typename _Val,
         typename _SparceContainer,
         typename _DenseContainer>
FastCharUnorderedMap<_Key,
                     _Val,
                     _SparceContainer,
                     _DenseContainer>::FastCharUnorderedMap(_Key maxKey)
    : maxKey_(maxKey),
      smallKeys_(maxKey, std::make_pair(_Val(), false)) {
}

template<typename _Key,
         typename _Val,
         typename _SparceContainer,
         typename _DenseContainer>
bool FastCharUnorderedMap<_Key,
                          _Val,
                          _SparceContainer,
                          _DenseContainer>::get(_Key key, _Val &val) {
    if (key <= maxKey_) {
        val = smallKeys_[key].first;
        return smallKeys_[key].second;
    } else {
        auto foundIt = mappedKeys_.find(key);
        if (foundIt == mappedKeys_.end())
            return false;
        val = foundIt->second;
        return true;
    }
}

template<typename _Key,
         typename _Val,
         typename _SparceContainer,
         typename _DenseContainer>
void FastCharUnorderedMap<_Key,
                          _Val,
                          _SparceContainer,
                          _DenseContainer>::set(_Key key, _Val val) {
    if (key < maxKey_) {
        smallKeys_[key].second = true;
        smallKeys_[key].first = val;
    } else {
        mappedKeys_.insert(std::make_pair(key, val));
    }
}
}
