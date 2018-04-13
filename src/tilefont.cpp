#include "tilefont.h"
#include <SDL2/SDL_image.h>

namespace rterm {
TileFont::TileFont()
    : Font() {
}

TileFont::TileFont(const std::string &path, size_t _w, size_t _h)
    : Font(),
      w_(_w),
      h_(_h) {
    static bool IMG_WasInit = false;
    if (!IMG_WasInit) {
        IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP);
        IMG_WasInit = true;
    }
    p_tilemap_ = SDL_Ptr<SDL_Surface>(IMG_Load(path.c_str()));
    if (!p_tilemap_.get()) {
        SDL_Log(IMG_GetError());
    }
}

TileFont& TileFont::operator=(TileFont &&rhs) {
    p_tilemap_ = std::move(rhs.p_tilemap_);
    p_tilemapTexture_.reset();
    w_ = rhs.w_;
    h_ = rhs.h_;

    return *this;
}

size_t TileFont::w() const {
    return w_;
}

size_t TileFont::h() const {
    return h_;
}

SDL_Rect TileFont::getRect(char_t c) const {
    size_t tilemapCols = p_tilemap_->w / w(),
           tilemapRows = p_tilemap_->h / h();
    SDL_Rect result{0, 0, 0, 0};
    if (c >= tilemapCols * tilemapRows)
        return result;

    result.y = h() * (c / tilemapCols);
    result.x = w() * (c % tilemapCols);
    result.h = h();
    result.w = w();
    
    return result;
}

void TileFont::render(SDL_Renderer *p_ren, SDL_Rect dst, char_t ch, Color fg, Color bg) {
    (void)fg;
    (void)bg;
    if (!p_tilemap_)
        return;
    if (!p_tilemapTexture_.get())
        p_tilemapTexture_ = SDL_Ptr<SDL_Texture>(
            SDL_CreateTextureFromSurface(p_ren, p_tilemap_.get())
        );
    SDL_SetRenderDrawColor(p_ren, bg.r(), bg.g(), bg.b(), bg.a());
    SDL_RenderFillRect(p_ren, &dst);
        
    SDL_Rect src = getRect(ch);
    SDL_RenderCopy(p_ren, p_tilemapTexture_.get(), &src, &dst);
}
}
