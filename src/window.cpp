#include "window.h"
#include "sdl_ptr.h"

namespace rterm {
Window::Window(int w, int h) {
    SDL_Window *win = SDL_CreateWindow(
        "rterm",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        w,
        h,
        0//SDL_WINDOW_OPENGL
    );
    window_ = makeSdlShared(win);
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    if (!ren)
        ren = SDL_CreateRenderer(win, -1, 0);
    renderer_ = makeSdlShared(ren);
    SDL_RenderClear(renderer_.get());
    SDL_RenderPresent(renderer_.get());
}

std::weak_ptr<SDL_Window> Window::window() {
    return std::weak_ptr<SDL_Window>(window_);
}

std::weak_ptr<SDL_Renderer> Window::renderer() {
    return std::weak_ptr<SDL_Renderer>(renderer_);
}
}
