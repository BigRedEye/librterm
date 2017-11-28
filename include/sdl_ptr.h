#ifndef RTERM_SDL_PTR_H
#define RTERM_SDL_PTR_H

#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

namespace rterm {

/// @cond custom_deleter

template<typename T>
struct sdl_deleter {
    void operator()(T* _ptr) {
        std::default_delete<T> del;
        del(_ptr);
    }
};

template< >
struct sdl_deleter<SDL_Window> {
    void operator()(SDL_Window* _ptr) {
        SDL_DestroyWindow(_ptr);
    }
};

template<>
struct sdl_deleter<SDL_Renderer> {
    void operator()(SDL_Renderer* _ptr) {
        SDL_DestroyRenderer(_ptr);
    }
};

template<>
struct sdl_deleter<SDL_Surface> {
    void operator()(SDL_Surface* _ptr) {
        SDL_FreeSurface(_ptr);
    }
};

template<>
struct sdl_deleter<SDL_Texture> {
    void operator()(SDL_Texture* _ptr) {
        SDL_DestroyTexture(_ptr);
    }
};

template<>
struct sdl_deleter<TTF_Font> {
    void operator()(TTF_Font* _ptr) {
        TTF_CloseFont(_ptr);
    }
};

/// @endcond

template<typename T>    
using SDL_Ptr = std::unique_ptr<T, sdl_deleter<T>>;

template<typename T>
using SDL_SharedPtr = std::shared_ptr<T>;

template<typename T>
inline SDL_SharedPtr<T> make_SDL_SharedPtr(T* ptr) {
    return SDL_SharedPtr<T>(ptr, sdl_deleter<T>());
}
}

#endif // RTERM_SDL_PTR_H
