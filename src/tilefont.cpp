#include "tilefont.h"
#include <SDL2/SDL_image.h>

namespace term {
TileFont::TileFont()
    : Font() {
}

TileFont::TileFont(const std::string &path, size_t _w, size_t _h)
    : w_(_w), h_(_h), p_tilemapTexture(NULL), Font() {
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
    p_tilemapTexture_ = SDL_Ptr<SDL_Window>(NULL);
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

SDL_Rect TileFont::getRect(char c) const {
    size_t tilemapCols = p_tilemap_->w / w(),
           tilemapRows = p_tilemap_->h / h();
    SDL_Rect result{0, 0, 0, 0};
    result.y = h() * (c / tilemapCols);
    result.x = w() * (c % tilemapCols);
    result.h = h();
    result.w = w();
    
    return result;
}

void TileFont::render(SDL_Renderer *p_ren, SDL_Rect dst, const char *str, Color fg, Color bg) {
    if (!p_tilemap_)
        return;
    if (!p_tilemapTexture_.get())
        p_tilemapTexture_ = SDL_Ptr<SDL_Window>(SDL_CreateTextureFromSurface(p_ren, p_tilemap_.get()));
    SDL_SetRenderDrawColor(p_ren, bg.r(), bg.g(), bg.b(), bg.a());
    SDL_RenderFillRect(p_ren, &dst);
    
    size_t len = strlen(str);
    for (int i = 0; i < len; ++i) {
        char c = str[i];
        
        SDL_Rect src = getRect(c);
        SDL_Rect charDst{0, 0, 0, 0};
        charDst.w = dst.w / len;
        charDst.x = dst.x + charDst.w * i;
        charDst.h = dst.h;
        charDst.y = dst.y;
        SDL_RenderCopy(p_ren, p_tilemapTexture, &src, &charDst);
    }
}
}
