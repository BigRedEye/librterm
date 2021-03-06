#include "rterm/rasterizer/tilefont.h"

#include "rterm/renderer/software_texture.h"

#include "rterm/util/error.h"
#include "rterm/util/logger.h"

namespace rterm {
TileFont::TileFont()
    : Font() {
}

TileFont::TileFont(const std::string& path, ui32 _w, ui32 _h)
    : Font()
    , w_(_w)
    , h_(_h) {
    p_tilemap_ = SdlHolder<SDL_Surface>(SoftwareTexture(path).release());
    if (!p_tilemap_.get()) {
        throw BadFont();
    }
}

TileFont& TileFont::operator=(TileFont&& rhs) {
    p_tilemap_ = std::move(rhs.p_tilemap_);
    p_tilemapTexture_.reset();
    w_ = rhs.w_;
    h_ = rhs.h_;

    return *this;
}

ui32 TileFont::w() const {
    return w_;
}

ui32 TileFont::h() const {
    return h_;
}

SDL_Rect TileFont::getRect(ch32 c) const {
    size_t tilemapCols = p_tilemap_->w / w();
    size_t tilemapRows = p_tilemap_->h / h();
    SDL_Rect result{0, 0, 0, 0};
    if (c >= tilemapCols * tilemapRows) {
        return result;
    }

    result.y = h() * (c / tilemapCols);
    result.x = w() * (c % tilemapCols);
    result.h = h();
    result.w = w();

    return result;
}

SoftwareTexture TileFont::render(ch32) {
    return {};
    /*
    if (!p_tilemap_) {
        return;
    }
    if (!p_tilemapTexture_.get()) {
        p_tilemapTexture_ = SdlHolder<SDL_Texture>(
            SDL_CreateTextureFromSurface(p_ren, p_tilemap_.get())
        );
    }
    SDL_SetRenderDrawColor(p_ren, bg.r(), bg.g(), bg.b(), bg.a());
    SDL_RenderFillRect(p_ren, &dst);

    SDL_Rect src = getRect(ch);
    SDL_RenderCopy(p_ren, p_tilemapTexture_.get(), &src, &dst);
    */
}

} // namespace rterm
