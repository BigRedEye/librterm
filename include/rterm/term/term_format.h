#pragma once

#include "rterm/util/util.h"

namespace rterm {

class TermFormat {
public:
    TermFormat(std::size_t w, std::size_t h)
        : w_(w)
        , h_(h) {
    }

    inline std::size_t w() const {
        return w_;
    }

    inline std::size_t h() const {
        return h_;
    }

private:
    ui64 w_;
    ui64 h_;
};

} // namespace rterm
