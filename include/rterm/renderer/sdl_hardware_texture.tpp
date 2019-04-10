#pragma once

#include "rterm/util/error.h"
#include "rterm/util/sdl/sdl_ptr.h"
#include "rterm/renderer/hardware_texture.h"
#include "rterm/renderer/software_texture.h"

namespace rterm {

template<>
class HardwareTextureBase<Api::SDL> {
public:
    using RawTextureRef = SDL_Texture*;

    HardwareTextureBase(RawTextureRef raw = nullptr)
        : holder_(new Holder(raw)) {
    }

    HardwareTextureBase(HardwareTextureBase&& other) {
        std::swap(other.holder_, holder_);
    }

    HardwareTextureBase(const HardwareTextureBase& other)
        : holder_(other.holder_) {
    }

    HardwareTextureBase& operator=(const HardwareTextureBase& other) {
        holder_ = other.holder_;
        return *this;
    }

    HardwareTextureBase& operator=(HardwareTextureBase&& other) {
        std::swap(holder_, other.holder_);
        return *this;
    }

    inline RawTextureRef get() const {
        return holder_->texture_;
    }

    inline void destroy() {
        delete holder_;
        holder_ = nullptr;
    }

    Vector<int, 2> size() const {
        if (!holder_) {
            return {0, 0};
        }
        if (!holder_->cachedSize_) {
            int w = 0;
            int h = 0;
            if (SDL_QueryTexture(get(), nullptr, nullptr, &w, &h) < 0) {
                throw BadTexture();
            }
            holder_->cachedSize_ = {w, h};
        }
        return holder_->cachedSize_;
    }

    int w() const {
        return size()[0];
    }

    int h() const {
        return size()[1];
    }

    void enableBlending() {
        SDL_SetTextureBlendMode(get(), SDL_BLENDMODE_BLEND);
    }

private:
    struct Holder {
        Holder(RawTextureRef texture = nullptr)
            : texture_(texture)
            , cachedSize_({0, 0}) {
        }

        ~Holder() {
			if (texture_) {
				SDL_DestroyTexture(texture_);
			}
        }

        RawTextureRef texture_ = nullptr;
        Vector<int, 2> cachedSize_;
    };

    Holder* holder_ = nullptr;
};

/* class that owns hardware (SDL in this case) texture */
template<>
class HardwareTexture<Api::SDL> {
public:
    using RawTextureRef = HardwareTextureBase<Api::SDL>::RawTextureRef;

    HardwareTexture() = default;

    /* use Renderer::createTexture instead */
    HardwareTexture(const HardwareTexture& other) = delete;
    HardwareTexture(HardwareTexture&& moved) = default;

    /* use Renderer::createTexture instead */
    HardwareTexture& operator=(const HardwareTexture& other) = delete;
    HardwareTexture& operator=(HardwareTexture&& moved) = default;

    HardwareTexture(RawTextureRef texture)
        : base_(texture) {
    }

    ~HardwareTexture() {
        base_.destroy();
    }

    inline HardwareTextureBase<Api::SDL>& base() {
        return base_;
    }

    inline const HardwareTextureBase<Api::SDL>& base() const {
        return base_;
    }

    inline RawTextureRef get() const {
        return base_.get();
    }

    int w() const {
        return base_.w();
    }

    int h() const {
        return base_.h();
    }

    Vector<i32, 2> size() const {
        return base_.size();
    }

    void enableBlending() {
        base_.enableBlending();
    }

private:
    HardwareTextureBase<Api::SDL> base_;
};

} // namespace rterm
