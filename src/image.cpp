#include "rterm/image.h"
#include "rterm/error.h"
#include "rterm/span.h"
#include "rterm/rect.h"

#include <stb_image.h>

#include <SDL_surface.h>
#include <SDL_pixels.h>

namespace rterm {

class InvalidImage final : public Exception {
public:
    InvalidImage(const std::string& what)
        : Exception(what)
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
    return load(path, SDL_PIXELFORMAT_RGBA32);
}

SDL_Surface* load(const std::string& path, ui32 pixelFormat) {
    int requiredFormat = STBI_rgb_alpha;
    Vector<int, 2> size{0, 0};
    int realFormat = 0;
    ui8* data = stbi_load(path.data(), &size[0], &size[1], &realFormat, requiredFormat);
    if (!data) {
        throw InvalidImage("Cannot load image: " + std::string(stbi_failure_reason()));
    }

    i32 depth = SDL_BITSPERPIXEL(pixelFormat);
    i32 bytesPerPixel = SDL_BYTESPERPIXEL(pixelFormat);

    size_t dataSize = bytesPerPixel * size[0] * size[1];
    span<ui8> pixels(data, dataSize);

    SDL_Surface* result = SDL_CreateRGBSurfaceWithFormat(0, size[0], size[1], depth, pixelFormat);

    RAIISurfaceLocker locker(result);
    std::copy(pixels.begin(), pixels.end(), static_cast<ui8*>(result->pixels));

    return result;
}

} // namespace rterm
