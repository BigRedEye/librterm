#pragma once


#include "api.h"
#include "sdl_loader.h"
#include "renderer.h"

#include <SDL2/SDL.h>

#include <memory>


namespace rterm {

class Window {
public:
    Window();
    Window(int w, int h);

    std::weak_ptr<SDL_Window> window() {
        return std::weak_ptr<SDL_Window>(window_);
    }

    Renderer<Api::api>& renderer() {
        return renderer_;
    }

    const Renderer<Api::api>& renderer() const {
        return renderer_;
    }

    void setTitle(const std::string& title);
    void setIcon(const SoftwareTexture& icon);
    void setMinimumSize(size_t width, size_t height);
    void setMaximumSize(size_t width, size_t height);

private:
    void initOpenGL();
    void initSDL();

private:
    std::shared_ptr<SDL_Window> window_;
    Renderer<Api::api> renderer_;
    SdlLoader loader_;
};

} // namespace rterm
