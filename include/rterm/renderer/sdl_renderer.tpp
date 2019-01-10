#pragma once

#include "rterm/util/color.h"
#include "rterm/util/error.h"
#include "rterm/renderer/renderer.h"

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

    Vector<i32, 2> maxTextureSize() {
        if (!info_) {
            info_ = std::make_unique<SDL_RendererInfo>();
            SDL_GetRendererInfo(get(), info_.get());
        }
        return {info_->max_texture_width, info_->max_texture_height};
    }

    void resizeBuffer(Vector<i32, 2> size) {
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

    SoftwareTexture dumpPixels(const iRect& rect) const {
        SoftwareTexture result(rect.w(), rect.h());
        SDL_Rect sdlRect = rect.sdl();
        SDL_RenderReadPixels(
            get(), &sdlRect, result.format, result.data(), SDL_BYTESPERPIXEL(result.format) * result.w());
        return result;
    }

    void flush() {
        present();
        if (buffered_) {
            clearBufferTarget();
            clear();
            render(buffer_, ScreenView());
            present();
            setBufferTarget();
        }
    }

    void present() {
        SDL_RenderPresent(get());
    }

    void clear() {
        if (SDL_RenderClear(get()) == -1) {
            throw BadRenderer();
        }
    }

    void clear(const TextureView<Api::SDL>& view) {
        setTarget(view);
        clear();
        present();
        setBufferTarget();
    }

    void render(const TextureView<Api::SDL>& src, const ScreenView& dst, const Color& mod) {
        setTextureColorMod(src, mod);
        render(src, dst);
    }

    void render(const TextureView<Api::SDL>& src, const ScreenView& dst) {
        SDL_Rect dstRect = dst.rect().sdl();
        SDL_Rect srcRect = src.rect().sdl();
        SDL_Rect* dstPtr = &dstRect;
        SDL_Rect* srcPtr = &srcRect;
        if (dst.empty()) {
            dstPtr = nullptr;
        }
        if (SDL_RenderCopy(get(), src.texture(), srcPtr, dstPtr) < 0) {
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
            if (rect.h == -1) {
                throw BadRenderer();
            }
            if (SDL_RenderFillRect(get(), &rect) < 0) {
                throw BadRenderer();
            }
        }
    }

    void fillRects(const Color& color, const std::vector<ScreenView>& dst) {
        setColor(color);
        std::vector<SDL_Rect> sdlDst;
        std::transform(dst.begin(), dst.end(), std::back_inserter(sdlDst), [](const ScreenView& view) {
            return view.rect().sdl();
        });
        if (SDL_RenderFillRects(get(), sdlDst.data(), sdlDst.size()) < 0) {
            throw BadRenderer();
        }
    }

    void copyTextureToTexture(const TextureView<Api::SDL>& src, const TextureView<Api::SDL>& dst) {
        setTarget(dst);
        render(src, dst.rect());
        present();
        setBufferTarget();
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
    std::unique_ptr<SDL_RendererInfo> info_;
};

} // namespace rterm
