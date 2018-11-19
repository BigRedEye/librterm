#pragma once

#include "util.h"

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
    std::size_t w_;
    std::size_t h_;
};

} // namespace rterm
