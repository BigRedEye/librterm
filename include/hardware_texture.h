#pragma once


#include "api.h"
#include "rect.h"
#include "software_texture.h"

#include <memory>


struct SDL_Texture;

namespace rterm {

template<Api::Type api>
class HardwareTextureBase : public std::enable_shared_from_this<HardwareTextureBase<api>> {
    friend class HardwareTexture;

private:
    HardwareTextureBase(const SoftwareTexture& softwareTexture);
};

template<>
class HardwareTextureBase<Api::GL> {
};

template<>
class HardwareTextureBase<Api::SDL> {
public:
    HardwareTextureBase(SDL_Texture* texture)
        : texture_(texture)
    {
    }

    inline SDL_Texture* ptr() const {
        return texture_.get();
    }

private:
    SdlPtr<SDL_Texture> texture_;
};

template<Api::Type api>
class HardwareTexture {
public:
    HardwareTexture() = default;
    HardwareTexture(const SoftwareTexture& softwareTexture);
    HardwareTexture(const HardwareTexture& other) = default;

    std::weak_ptr<HardwareTextureBase<api>> get() const {
        return std::weak_ptr<HardwareTextureBase<api>>(base_);
    }
    
private:
    std::shared_ptr<HardwareTextureBase<api>> base_;
};

template<>
class HardwareTexture<Api::SDL> {
    HardwareTexture(const SoftwareTexture& softwareTexture);
};

} // namespace rterm
