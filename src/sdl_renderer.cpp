#include "sdl_renderer.h"


namespace rterm {

using SdlRenderer = Renderer<Api::SDL>;

void SdlRenderer::clear() const {
    SDL_RenderClear(get());
}

void SdlRenderer::flush() const {
    SDL_RenderPresent(get());
}

void SdlRenderer::draw(const TextureView<Api::SDL>& src, const ScreenView& dst) {
    SDL_Rect srcrect = src.rect().sdl();
    SDL_Rect dstrect = dst.rect().sdl();
    SDL_RenderCopy(get(), src.texture().lock()->ptr(), &srcrect, &dstrect);
}

} // namespace rterm
