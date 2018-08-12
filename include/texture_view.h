#pragma once


#include "api.h"
#include "hardware_texture.h"
#include "rect.h"


namespace rterm {

template<Api::Type api>
class TextureView {
public:
    TextureView(const HardwareTexture<api>& src)
        : texture_(src.base())
        , rect_({0, 0}, texture_.size()) {
    }

    TextureView(const HardwareTexture<api>& src, const iRect& rect)
        : texture_(src.base())
        , rect_(rect) {
    }

    inline const iRect& rect() const {
        return rect_;
    }

    typename HardwareTextureBase<api>::RawTextureRef texture() const {
        return texture_.get();
    }

private:
    HardwareTextureBase<api> texture_;
    iRect rect_;
};

} // namespace rterm
