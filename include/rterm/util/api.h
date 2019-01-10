#pragma once

#include "rterm/util/util.h"

namespace rterm {

enum class VApi : ui8 {
#if RTERM_HAS_OGL
    Gl = 0,
#endif
#if RTERM_HAS_SDL
    Sdl = 1,
#endif
};

} // namespace rterm

namespace rterm {

struct Api {
    enum Type : ui8 {
        GL = 0,
        SDL = 1,
    };

#if RTERM_HAS_OGL
    static constexpr Type api = SDL;
#else
    static constexpr Type api = SDL;
#endif
};

} // namespace rterm
