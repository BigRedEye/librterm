#include "software_texture.h"

#include <cstring>


namespace rterm {

constexpr uint32_t SoftwareTexture::format;

SoftwareTexture::SoftwareTexture()
{
}

SoftwareTexture::SoftwareTexture(const SdlPtr<SDL_Surface>& surface)
    : SoftwareTexture()
{
    if (!surface) {
        return;
    }
    surface_ = SdlPtr<SDL_Surface>(SDL_ConvertSurfaceFormat(surface.get(), format, 0));
}

} // namespace rterm
