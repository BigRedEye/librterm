#pragma once


#include "sdl_ptr.h"


namespace rterm {

template<typename Raw>
class Renderer {
public:
    Renderer() = delete;

    Renderer(const Raw& raw);

    virtual ~Renderer() = default;

    inline Raw& raw() const {
        return raw_;
    }

private:
    Raw raw_;
};

} // namespace rterm
