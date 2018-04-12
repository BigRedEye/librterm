#include "event.h"

namespace rterm {
inline int eventTypeFromSDLEvent(SDL_Event *event) {
    int type = EventType::Unknown;
    switch (event->type) {
    case SDL_QUIT:
        type = EventType::Quit;
        break;
    case SDL_WINDOWEVENT:
        switch (event->window.event) {
        case SDL_WINDOWEVENT_HIDDEN:
            type = EventType::WindowHidden;
            break;
        case SDL_WINDOWEVENT_SHOWN:
            type = EventType::WindowShown;
            break;
        case SDL_WINDOWEVENT_MOVED:
            type = EventType::WindowMoved;
            break;
        case SDL_WINDOWEVENT_RESIZED:
            type = EventType::WindowResized;
            break;
        default:
            type = EventType::Unknown;
            break;
        }
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
    type_(eventTypeFromSDLEvent(event)) {
}

int Event::type() const {
    return type_;
}
}
