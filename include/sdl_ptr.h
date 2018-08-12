#pragma once


#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <memory>


namespace rterm {

/// @cond INTERNAL

template<typename T>
struct SdlDeleter {
    void operator()(T* _ptr) {
        std::default_delete<T> del;
        del(_ptr);
    }
};

template<>
struct SdlDeleter<SDL_Window> {
    void operator()(SDL_Window* _ptr) {
        SDL_DestroyWindow(_ptr);
    }
};

template<>
struct SdlDeleter<SDL_Renderer> {
    void operator()(SDL_Renderer* _ptr) {
        SDL_DestroyRenderer(_ptr);
    }
};

template<>
struct SdlDeleter<SDL_Surface> {
    void operator()(SDL_Surface* _ptr) {
        SDL_FreeSurface(_ptr);
    }
};

template<>
struct SdlDeleter<SDL_Texture> {
    void operator()(SDL_Texture* _ptr) {
        SDL_DestroyTexture(_ptr);
    }
};

template<>
struct SdlDeleter<TTF_Font> {
    void operator()(TTF_Font* _ptr) {
        TTF_CloseFont(_ptr);
    }
};

template<typename T>
using SdlHolder = std::unique_ptr<T, SdlDeleter<T>>;

template<typename T>
using SdlSharedPtr = std::shared_ptr<T>;

template<typename T>
inline SdlSharedPtr<T> makeSdlShared(T* ptr) {
    return SdlSharedPtr<T>(ptr, SdlDeleter<T>());
}

/// @INTERNAL

} // namespace rterm
