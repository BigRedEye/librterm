#ifndef RTERM_SDL_LOADER_H
#define RTERM_SDL_LOADER_H

namespace rterm {

/**
 * @brief RAII-styled loader for SDL libraries
 */

class SdlLoader {
public:
    SdlLoader();
    ~SdlLoader();

private:
    static int countOfLoads_;
};

}

#endif // RTERM_SDL_LOADER_H
