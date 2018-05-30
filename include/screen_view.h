#pragma once


#include "rect.h"


struct SDL_Rect;


namespace rterm {

class ScreenView {
public:
    ScreenView(const iRect& rect)
        : rect_(rect)
    {
    }

    ScreenView(const SDL_Rect& rect)
        : ScreenView(iRect{rect.x, rect.y, rect.w, rect.h})
    {
    }

    ScreenView(int x, int y, int w, int h)
        : ScreenView(iRect{x, y, w, h})
    {
    }
    
    const iRect& rect() const {
        return rect_;
    }
    
    iRect& rect() {
        return rect_;
    }

private:
    iRect rect_;
};

} // namespace rterm
