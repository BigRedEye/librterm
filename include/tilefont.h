#ifndef LIBTERM_TILEFONT_H
#define LIBTERM_TILEFONT_H

#include "font.h"
#include "sdl_ptr.h"

namespace term {

class TileFont : public Font {
public:
    TileFont();
    TileFont(const std::string &path, size_t _w, size_t _h);

    TileFont& operator=(TileFont &&rhs);

    virtual void render(SDL_Renderer *p_ren, SDL_Rect dst, const char *str, Color fg, Color bg) override;

    virtual size_t w() const override;
    virtual size_t h() const override;

private:
    SDL_Rect getRect(char c) const;
    
    SDL_Ptr<SDL_Surface> p_tilemap_;
    SDL_Ptr<SDL_Texture> p_tilemapTexture_;
    size_t w_, h_;
};
}

#endif // LIBTERM_TILEFONT_H
