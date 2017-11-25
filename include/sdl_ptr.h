#ifndef LIBTERM_SDL_PTR_H
#define LIBTERM_SDL_PTR_H

#include <memory>
#include <SDL2/SDL.h>
#include <iostream>

namespace term {

template<typename T>
struct sdl_deleter {
    void operator()(T* _ptr) {
        std::default_delete<T> del;
        del(_ptr);
        std::cerr << "Default deleter called" << std::endl;
    }
};

template< >
struct sdl_deleter<SDL_Window> {
    void operator()(SDL_Window* _ptr) {
        SDL_DestroyWindow(_ptr);
        std::cerr << "SDL_DestroyWindow() called" << std::endl;
    }
};

template<>
struct sdl_deleter<SDL_Renderer> {
    void operator()(SDL_Renderer* _ptr) {
        SDL_DestroyRenderer(_ptr);
        std::cerr << "SDL_DestroyRenderer() called" << std::endl;
    }
};

template<>
struct sdl_deleter<SDL_Surface> {
    void operator()(SDL_Surface* _ptr) {
        SDL_FreeSurface(_ptr);
        std::cerr << "SDL_FreeSurface() called" << std::endl;
    }
};

template<>
struct sdl_deleter<SDL_Texture> {
    void operator()(SDL_Texture* _ptr) {
        SDL_DestroyTexture(_ptr);
        std::cerr << "SDL_DestroyTexture() called" << std::endl;
    }
};

template<typename T>    
using SDL_Ptr = std::unique_ptr<T, sdl_deleter<T>>;
}

#endif // LIBTERM_SDL_PTR_H
