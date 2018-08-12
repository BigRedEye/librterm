#include "glyph_cache.h"
#include "ttfont.h"
#include "tilefont.h"


namespace rterm {

void GlyphCache::setFont(const std::string &path, size_t sz) {
    reset(std::make_unique<TTFont>(path, sz));
}

void GlyphCache::setFont(const std::string &path, size_t w, size_t h) {
    reset(std::make_unique<TileFont>(path, w, h));
}

int GlyphCache::w() {
    return font_->w();
}

int GlyphCache::h() {
    return font_->h();
}

TextureView<Api::api> GlyphCache::getGlyph(char_t ch) {
    CacheHit hit;
    if (cache_.mapping.has(ch)) {
        hit = cache_.mapping.get(ch);
    } else {
        SoftwareTexture stex = font_->render(ch);
        HardwareTexture<Api::SDL> texture = renderer_.createTexture(stex);
        iRect rect({0, 0}, texture.size());
        hit = CacheHit{cache_.textures.size(), std::move(rect)};
        cache_.mapping.set(ch, hit);
        cache_.textures.emplace_back(std::move(texture));
    }
    return TextureView<Api::api>(cache_.textures[hit.texture], hit.rect);
}

} // namespace rterm
