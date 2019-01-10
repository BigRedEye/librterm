#include "rterm/sdl_loader.h"
#include "rterm/error.h"

#include <SDL.h>

#ifndef RTERM_WITHOUT_TTF
#include <SDL_ttf.h>
#else
int TTF_Init(int) {
    return 0;
}

void TTF_Quit() {
}

#endif // RTERM_WITHOUT_TTF

namespace rterm {

int SdlLoader::countOfLoads_ = 0;

SdlLoader::SdlLoader() {
    if (countOfLoads_ == 0) {
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
            throw Exception();
        }
        if (TTF_Init() < 0) {
            throw Exception();
        }
    }
    ++countOfLoads_;
}

SdlLoader::~SdlLoader() {
    --countOfLoads_;
    if (countOfLoads_ == 0) {
        TTF_Quit();
        SDL_Quit();
    }
}

} // namespace rterm
