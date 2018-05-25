#pragma once


#include "api.h"
#include "rect.h"
#include "software_texture.h"


namespace rterm {

template<Api::Type api>
class HardwareTexture;

template<>
class HardwareTexture<Api::GL> {
    
};

template<>
class HardwareTexture<Api::SDL> {
public:
    HardwareTexture(const SoftwareTexture<PixelRgba32>& softwareTexture);
};

} // namespace rterm
