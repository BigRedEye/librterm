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

void Window::setTitle(const std::string& title) {
    SDL_SetWindowTitle(window_.get(), title.c_str());
}

void Window::setIcon(const SoftwareTexture &icon) {
    SDL_SetWindowIcon(window_.get(), icon.get());
}

void Window::setMinimumSize(size_t width, size_t height) {
    SDL_SetWindowMinimumSize(window_.get(), width, height);
}

void Window::setMaximumSize(size_t width, size_t height) {
    SDL_SetWindowMaximumSize(window_.get(), width, height);
}

void Window::initSDL() {
    SDL_Renderer* ren = SDL_CreateRenderer(
        window_.get(),
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE
    );
    if (!ren) {
        ren = SDL_CreateRenderer(window_.get(), -1, 0);
    }
    renderer_ = Renderer<Api::SDL>(ren);
    renderer_.clear();
    renderer_.flush();
}

void Window::initOpenGL() {
    SDL_GLContext context = SDL_GL_CreateContext(window_.get());
    (void)context;
}

} // namespace rterm
