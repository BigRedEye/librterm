#pragma once

#include "color.h"
#include "error.h"
#include "renderer.h"

#include <SDL2/SDL_image.h>

namespace rterm {

template<>
class Renderer<Api::SDL> {
public:
    explicit Renderer(SDL_Renderer* renderer = nullptr, bool buffered = true)
        : buffered_(buffered)
        , renderer_(renderer) {
        if (renderer_) {
            if (buffered_) {
                i32 w;
                i32 h;
                SDL_GetRendererOutputSize(get(), &w, &h);
                resizeBuffer({w, h});
            }
            setBufferTarget();
        }
    }

    inline SDL_Renderer* get() const {
        return renderer_.get();
    }

    HardwareTexture<Api::SDL> createTexture(int w, int h) {
        return createTexture(Vector<int, 2>{w, h});
    }

    /* copy 8 bytes */
    HardwareTexture<Api::SDL> createTexture(Vector<int, 2> size) {
        SDL_Texture* raw =
            SDL_CreateTexture(get(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, size[0], size[1]);
        if (!raw) {
            throw BadTexture();
        }
        return HardwareTexture<Api::SDL>(raw);
    }

    HardwareTexture<Api::SDL> createTexture(const TextureView<Api::SDL>& src) {
        HardwareTexture<Api::SDL> empty = createTexture(src.rect().w(), src.rect().h());
        setTarget(empty);
        render(src, empty.size());
        clearTarget();
        return empty;
    }

    HardwareTexture<Api::SDL> createTexture(const SoftwareTexture& src) {
        SDL_Texture* raw = SDL_CreateTextureFromSurface(get(), src.get());
        if (!raw) {
            throw BadTexture();
        }
        return HardwareTexture<Api::SDL>(raw);
    }

    void resizeBuffer(Vector<int, 2> size) {
        if (!buffered_) {
            return;
        }
        HardwareTexture<Api::SDL> previous = std::move(buffer_);
        buffer_ = createTexture(size);
        setBufferTarget();
        if (previous.get()) {
            ScreenView dst(0, 0, 0, 0);
            dst.rect().w() = previous.w();
            dst.rect().h() = previous.h();
            render(previous, dst);
        }
    }

    void flush() {
        SDL_RenderPresent(get());
        if (buffered_) {
            clearBufferTarget();
            SDL_RenderClear(get());
            if (SDL_RenderCopy(get(), buffer_.get(), nullptr, nullptr)) {
                throw BadRenderer();
            }
            SDL_RenderPresent(get());
            setBufferTarget();
        }
    }

    void clear() {
        if (SDL_RenderClear(get()) == -1) {
            throw BadRenderer();
        }
    }

    void render(const TextureView<Api::SDL>& src, const ScreenView& dst, const Color& mod) {
        setTextureColorMod(src, mod);
        render(src, dst);
    }

    void render(const TextureView<Api::SDL>& src, const ScreenView& dst) {
        SDL_Rect dstRect = dst.rect().sdl();
        SDL_Rect srcRect = src.rect().sdl();
        if (SDL_RenderCopy(get(), src.texture(), &srcRect, &dstRect) < 0) {
            throw BadRenderer();
        }
    }

    void setColor(const Color& color) {
        if (SDL_SetRenderDrawColor(get(), color.r(), color.g(), color.b(), color.a()) < 0) {
            throw BadRenderer();
        }
    }

    void setTextureColorMod(const TextureView<Api::SDL>& src, const Color& color) {
        if (SDL_SetTextureColorMod(src.texture(), color.r(), color.g(), color.b()) < 0) {
            throw BadTexture();
        }
    }

    void fill(const Color& color, const ScreenView& dst = ScreenView()) {
        setColor(color);
        if (dst.rect() == iRect{0, 0, 0, 0}) {
            if (SDL_RenderClear(get()) < 0) {
                throw BadRenderer();
            }
        } else {
            SDL_Rect rect = dst.rect().sdl();
            if (SDL_RenderFillRect(get(), &rect) < 0) {
                throw BadRenderer();
            }
        }
    }

private:
    void setBufferTarget() {
        if (buffered_) {
            setTarget(buffer_);
        }
    }

    void setTarget(const TextureView<Api::SDL>& view) {
        if (SDL_SetRenderTarget(get(), view.texture()) < 0) {
            throw BadRenderer();
        }
    }

    void clearTarget() {
        if (SDL_SetRenderTarget(get(), nullptr) < 0) {
            throw BadRenderer();
        }
    }

    void clearBufferTarget() {
        if (buffered_) {
            clearTarget();
        }
    }

private:
    bool buffered_;
    SdlHolder<SDL_Renderer> renderer_;
    HardwareTexture<Api::SDL> buffer_;
};

} // namespace rterm
