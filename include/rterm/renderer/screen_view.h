#pragma once

#include "rterm/util/rect.h"
#include <SDL_rect.h>

namespace rterm {

class ScreenView {
public:
    ScreenView()
        : rect_(0, 0, 0, 0) {
    }

    ScreenView(Vector<i32, 2> size)
        : rect_(0, 0, size[0], size[1]) {
    }

    ScreenView(const iRect& rect)
        : rect_(rect) {
    }

    ScreenView(const SDL_Rect& rect)
        : ScreenView(iRect{rect.x, rect.y, rect.w, rect.h}) {
    }

    ScreenView(i32 x, i32 y, i32 w, i32 h)
        : ScreenView(iRect{x, y, w, h}) {
    }

    const iRect& rect() const {
        return rect_;
    }

    iRect& rect() {
        return rect_;
    }

    bool empty() const {
        return rect().w() == 0 || rect().h() == 0;
    }

private:
    iRect rect_;
};

} // namespace rterm
