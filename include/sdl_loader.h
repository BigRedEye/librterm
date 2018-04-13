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
    template<typename F>
    void load(F&& f, int flags);
    template<typename F>
    void unload(F&& f, int flags);

    static int countOfLoads_;
};

}

#endif // RTERM_SDL_LOADER_H
