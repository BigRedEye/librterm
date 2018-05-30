#pragma once


#include "api.h"
#include "hardware_texture.h"
#include "rect.h"


namespace rterm {

template<Api::Type api>
class TextureView {
public:
    TextureView(const HardwareTexture<api>& src, const iRect& rect)
        : texture_(src.get())
        , rect_(rect) {
    }

    inline const iRect& rect() const {
        return rect_;
    }

    inline iRect& rect() {
        return rect_;
    }

    const std::weak_ptr<HardwareTextureBase<api>>& texture() const {
        return texture_;
    }

private:
    std::weak_ptr<HardwareTextureBase<api>> texture_;
    iRect rect_;
};

} // namespace rterm
