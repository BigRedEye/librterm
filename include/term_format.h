#pragma once


#include <stdint.h>


namespace rterm {

class TermFormat {
public:
    TermFormat(size_t w, size_t h)
        : w_(w)
        , h_(h)
    {
    }

    inline size_t w() const {
        return w_;
    }

    inline size_t h() const {
        return h_;
    }

private:
    size_t w_;
    size_t h_;
};

} // namespace rterm
