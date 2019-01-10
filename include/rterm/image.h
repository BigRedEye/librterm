#pragma once

#include "rterm/util.h"

#include <string>

struct SDL_Surface;

namespace rterm {

SDL_Surface* load(const std::string& path, ui32 pixelFormat);
SDL_Surface* load(const std::string& path);

} // namespace rterm
