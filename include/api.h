#pragma once

#include "util.h"

namespace rterm {

struct Api {
    enum Type : ui8 {
        GL = 0,
        SDL = 1,
    };

#ifdef WITHOUT_OPENGL
    static constexpr Type api = SDL;
#else
    static constexpr Type api = SDL;
#endif
};

} // namespace rterm
