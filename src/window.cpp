#include "window.h"
#include "api.h"
#include "sdl_ptr.h"

namespace rterm {

Window::Window(int w, int h) {
    Uint32 flags = 0;
    if (Api::api == Api::GL) {
        flags |= SDL_WINDOW_OPENGL;
    }
    SDL_Window* win = SDL_CreateWindow("rterm", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, flags);
    if (!win) {
        throw Exception();
    }
    window_.reset(win);
    if (Api::api == Api::GL) {
        initOpenGL();
    } else {
        initSDL();
    }
}

void Window::resize(int width, int height) {
    resize({width, height});
}

void Window::resize(Vector<int, 2> newSize) {
    Vector<int, 2> currentSize = size();
    if (currentSize != newSize) {
        SDL_SetWindowSize(get(), newSize[0], newSize[1]);
    }
    renderer_.resizeBuffer(newSize);
}

void Window::setTitle(const std::string& title) {
    SDL_SetWindowTitle(get(), title.c_str());
}

void Window::setIcon(const SoftwareTexture& icon) {
    SDL_SetWindowIcon(get(), icon.get());
}

void Window::setMinimumSize(int width, int height) {
    SDL_SetWindowMinimumSize(get(), width, height);
}

void Window::setMaximumSize(int width, int height) {
    SDL_SetWindowMaximumSize(get(), width, height);
}

Vector<int, 2> Window::size() const {
    Vector<int, 2> wh;
    SDL_GetWindowSize(window_.get(), &wh[0], &wh[1]);
    return wh;
}

void Window::initSDL() {
    SDL_Renderer* ren = SDL_CreateRenderer(get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    if (!ren) {
        ren = SDL_CreateRenderer(window_.get(), -1, 0);
    }
    if (!ren) {
        throw Exception();
    }
    renderer_ = Renderer<Api::SDL>(ren);
}

void Window::initOpenGL() {
    SDL_GLContext context = SDL_GL_CreateContext(window_.get());
    (void)context;
}

} // namespace rterm
