#pragma once

#include "rterm/util.h"

#include <string>

#include <SDL_pixels.h>
#include <SDL_version.h>

#if !SDL_VERSION_ATLEAST(2, 0, 5)
enum {
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    SDL_PIXELFORMAT_RGBA32 = SDL_PIXELFORMAT_RGBA8888,
    SDL_PIXELFORMAT_ARGB32 = SDL_PIXELFORMAT_ARGB8888,
    SDL_PIXELFORMAT_BGRA32 = SDL_PIXELFORMAT_BGRA8888,
    SDL_PIXELFORMAT_ABGR32 = SDL_PIXELFORMAT_ABGR8888,
#else
    SDL_PIXELFORMAT_RGBA32 = SDL_PIXELFORMAT_ABGR8888,
    SDL_PIXELFORMAT_ARGB32 = SDL_PIXELFORMAT_BGRA8888,
    SDL_PIXELFORMAT_BGRA32 = SDL_PIXELFORMAT_ARGB8888,
    SDL_PIXELFORMAT_ABGR32 = SDL_PIXELFORMAT_RGBA8888,
#endif
};
#endif

struct SDL_Surface;

namespace rterm {

SDL_Surface* load(const std::string& path, ui32 pixelFormat);
SDL_Surface* load(const std::string& path);

} // namespace rterm
