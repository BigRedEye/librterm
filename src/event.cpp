#include "event.h"

namespace rterm {
inline int eventTypeFromSDLEvent(SDL_EventType sdltype) {
    int type = 0;
    switch (sdltype) {
    case SDL_QUIT:
        type = EventType::Quit;
        break;
    case SDL_WINDOWEVENT:
        type = EventType::Window;
        break;
    case SDL_SYSWMEVENT:
        type = EventType::System;
        break;
    case SDL_KEYDOWN:
        type = EventType::KeyDown;
        break;
    case SDL_KEYUP:
        type = EventType::KeyUp;
        break;
    case SDL_MOUSEMOTION:
        type = EventType::MouseMove;
        break;
    case SDL_MOUSEBUTTONDOWN:
        type = EventType::MouseDown;
        break;
    case SDL_MOUSEBUTTONUP:
        type = EventType::MouseUp;
        break;
    case SDL_MOUSEWHEEL:
        type = EventType::MouseWheel;
        break;
    default:
        type = EventType::Unknown;
        break;
    }

    return type;
}

Event::Event(SDL_Event *event) :
    type_(eventTypeFromSDLEvent(static_cast<SDL_EventType>(event->type))) {
}

int Event::type() const {
    return type_;
}
}
