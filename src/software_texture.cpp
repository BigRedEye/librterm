#include "error.h"
#include "software_texture.h"

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

SoftwareTexture::SoftwareTexture(const SoftwareTexture &other)
    : SoftwareTexture()
{
    *this = other;
}

SoftwareTexture::SoftwareTexture(SDL_Surface *surface)
    : surface_(surface)
{
}

SoftwareTexture::SoftwareTexture(int w, int h) {
    SDL_Surface* raw = SDL_CreateRGBSurfaceWithFormat(
        0, w, h, 32,
        SDL_PIXELFORMAT_RGBA32
    );
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
