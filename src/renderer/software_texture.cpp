#include "rterm/renderer/software_texture.h"

#include "rterm/util/error.h"
#include "rterm/util/error.h"
#include "rterm/util/span.h"

#include <stb_image.h>

#include <cstring>

namespace {

SDL_Surface* sdlCopySurface(SDL_Surface* surface) {
    if (!surface) {
        throw std::runtime_error("Cannot copy surface at nullptr");
    }
    SDL_Surface* res = SDL_ConvertSurface(surface, surface->format, 0);
    if (!res) {
        throw rterm::Exception();
    }
    return res;
}

class InvalidImage final : public rterm::Exception {
public:
    InvalidImage(const std::string& what)
        : rterm::Exception(what)
    {}
};

class RAIISurfaceLocker {
public:
    RAIISurfaceLocker(SDL_Surface* ptr)
        : ptr_(ptr) {
        SDL_LockSurface(ptr_);
    }

    ~RAIISurfaceLocker() {
        SDL_UnlockSurface(ptr_);
    }

private:
    SDL_Surface* ptr_;
};

SDL_Surface* load(const std::string& path) {
    using namespace rterm;

    int requiredFormat = STBI_rgb_alpha;
    Vector<int, 2> size{0, 0};
    int realFormat = 0;
    ui8* data = stbi_load(path.data(), &size[0], &size[1], &realFormat, requiredFormat);
    if (!data) {
        throw InvalidImage("Cannot load image: " + std::string(stbi_failure_reason()));
    }

    const ui32 format = SoftwareTexture::format;
    const i32 bytesPerPixel = SDL_BYTESPERPIXEL(format);

    const size_t dataSize = bytesPerPixel * size[0] * size[1];
    const span<ui8> pixels(data, dataSize);

    SDL_Surface* result = SoftwareTexture(size[0], size[1]).release();

    RAIISurfaceLocker locker(result);
    std::copy(pixels.begin(), pixels.end(), static_cast<ui8*>(result->pixels));

    stbi_image_free(data);

    return result;
}

} // anonymous namespace

namespace rterm {

constexpr uint32_t SoftwareTexture::format;

SoftwareTexture::SoftwareTexture(const SoftwareTexture& other)
    : SoftwareTexture() {
    *this = other;
}

SoftwareTexture::SoftwareTexture(const std::string& path)
    : SoftwareTexture(load(path)) {
}

SoftwareTexture::SoftwareTexture(SDL_Surface* surface)
    : surface_(surface) {
}

SoftwareTexture::SoftwareTexture(i32 w, i32 h) {
#if SDL_VERSION_ATLEAST(2, 0, 5)
    SDL_Surface* raw = SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, format);
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

void SoftwareTexture::save(const std::string& file) {
    if (!surface_) {
        throw BadTexture();
    }

    SDL_SaveBMP(surface_.get(), file.data());
}

} // namespace rterm
