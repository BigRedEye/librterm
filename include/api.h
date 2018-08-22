#pragma once

namespace rterm {

struct Api {
    enum Type : unsigned char {
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
