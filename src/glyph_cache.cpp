#include "rterm/glyph_cache.h"
#include "rterm/tilefont.h"
#include "rterm/ttfont.h"

namespace rterm {

void GlyphCache::setFont(const std::string &path, ui32 sz) {
    reset(std::make_unique<TTFont>(path, sz));
}

void GlyphCache::setFont(const std::string &path, ui32 w, ui32 h) {
    reset(std::make_unique<TileFont>(path, w, h));
}

i32 GlyphCache::w() {
    return font_->w();
}

i32 GlyphCache::h() {
    return font_->h();
}

TextureView<Api::api> GlyphCache::getGlyph(ch32 ch) {
    CacheHit hit;
    if (cache_.mapping.has(ch)) {
        hit = cache_.mapping.get(ch);
    } else {
        SoftwareTexture stex = font_->render(ch);
        writeGlyph(stex);
        iRect rect(cache_.nextEmpty_, stex.size());
        hit = CacheHit{cache_.textures.size() - 1, std::move(rect)};
        Vector<i32, 2> cacheSize = cache_.textures.back().size();
        cache_.nextEmpty_[0] += w();
        if (cache_.nextEmpty_[0] + w() >= cacheSize[0]) {
            cache_.nextEmpty_[0] = 0;
            cache_.nextEmpty_[1] += h();
            if (cache_.nextEmpty_[1] + h() >= cacheSize[1]) {
                addAtlas();
            }
        }
        cache_.mapping.set(ch, hit);
    }
    return TextureView<Api::api>(cache_.textures[hit.texture], hit.rect);
}

void GlyphCache::addAtlas() {
    Vector<i32, 2> size = renderer_.maxTextureSize();
    size[0] = std::min(size[0], 4096);
    size[1] = std::min(size[1], 4096);
    size[0] -= size[0] % w();
    size[1] -= size[1] % h();
    cache_.textures.push_back(renderer_.createTexture(size));
    renderer_.setColor({0xff, 0xff, 0xff, 0x00});
    renderer_.clear(cache_.textures.back());
    cache_.textures.back().enableBlending();
    cache_.nextEmpty_ = {0, 0};
}

void GlyphCache::writeGlyph(const SoftwareTexture &glyph) {
    iRect dst{cache_.nextEmpty_, glyph.size()};
    HardwareTexture<Api::SDL> glyphTexture = renderer_.createTexture(glyph);
    TextureView<Api::SDL> view(glyphTexture);
    renderer_.copyTextureToTexture(view, {cache_.textures.back(), dst});
}

} // namespace rterm
