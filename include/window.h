#ifndef RTERM_WINDOW_H
#define RTERM_WINDOW_H

#include "sdl_loader.h"
#include <memory>
#include <SDL2/SDL.h>

namespace rterm {
class Window {
public:
    Window();
    Window(int w, int h);

    std::weak_ptr<SDL_Window> window();
    std::weak_ptr<SDL_Renderer> renderer();

private:
    std::shared_ptr<SDL_Window> window_;
    std::shared_ptr<SDL_Renderer> renderer_;
    SdlLoader loader_;
};
}

#endif // RTERM_WINDOW_H

