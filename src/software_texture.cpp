#include "software_texture.h"
#include "error.h"

#include <cstring>

namespace {

inline SDL_Surface* sdlCopySurface(SDL_Surface* surface) {
    if (!surface) {
        throw std::runtime_error("Cannot copy surface at nullptr");
    }
    SDL_Surface* res = SDL_ConvertSurface(surface, surface->format, 0);
    if (!res) {
        throw rterm::Exception();
    }
    return res;
}

} // anonymous namespace

namespace rterm {

constexpr uint32_t SoftwareTexture::format;

SoftwareTexture::SoftwareTexture(const SoftwareTexture& other)
    : SoftwareTexture() {
    *this = other;
}

SoftwareTexture::SoftwareTexture(SDL_Surface* surface)
    : surface_(surface) {
}

SoftwareTexture::SoftwareTexture(int w, int h) {
#if SDL_VERSION_ATLEAST(2, 0, 5)
    SDL_Surface* raw = SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, SDL_PIXELFORMAT_RGBA32);
#else
    Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif
    SDL_Surface* raw = SDL_CreateRGBSurface(0, w, h, 32, rmask, gmask, bmask, amask);
#endif
    if (!raw) {
        throw BadTexture();
    }
    surface_.reset(raw);
}

SoftwareTexture& SoftwareTexture::operator=(const SoftwareTexture& other) {
    surface_.reset(::sdlCopySurface(other.get()));
    return *this;
}

} // namespace rterm
