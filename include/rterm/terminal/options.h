#pragma once

#include <rterm/util/api.h>
#include <rterm/util/defs.h>

namespace rterm::terminal {

namespace detail {} // namespace detail

class Options {
public:
    template<typename... Args>
    Options(Args&&... args) {
        (fillOptions(std::forward<Args>(args)) + ... + 0);
    }

private:
    int fillOptions()

        private : u32 width_;
    u32 height_;
    bool fullscreen_;
    bool resizeable_;
    Api apis_;
};

} // namespace rterm::terminal
