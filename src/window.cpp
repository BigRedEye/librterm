#include "api.h"
#include "sdl_ptr.h"
#include "window.h"


namespace rterm {

Window::Window(int w, int h) {
    Uint32 flags = 0;
    if (Api::api == Api::GL)
        flags |= SDL_WINDOW_OPENGL;
    SDL_Window* win = SDL_CreateWindow(
        "rterm",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        w,
        h,
        flags
    );
    window_ = makeSdlShared(win);
    if (Api::api == Api::GL) {
        initOpenGL();
    } else {
        initSDL();
    }
}

void Window::initSDL() {
    SDL_Renderer* ren = SDL_CreateRenderer(
        win,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE
    );
    if (!ren) {
        ren = SDL_CreateRenderer(win, -1, 0);
    }
    renderer_ = makeSdlShared(ren);
    SDL_RenderClear(renderer_.get());
    SDL_RenderPresent(renderer_.get());
}

void Window::initOpenGL() {
    SDL_GLContext = SDL_GL_CreateContext(window_.get());
}

std::weak_ptr<SDL_Window> Window::window() {
    return std::weak_ptr<SDL_Window>(window_);
}

std::weak_ptr<SDL_Renderer> Window::renderer() {
    return std::weak_ptr<SDL_Renderer>(renderer_);
}

} // namespace rterm
