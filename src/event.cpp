#include "event.h"
#include "logger.h"

namespace rterm {

namespace events {

inline i32 eventTypeFromSDLEvent(SDL_Event* event) noexcept {
    i32 type = EventType::Unknown;
    if (!event) {
        return type;
    }
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
        case SDL_WINDOWEVENT_EXPOSED:
            type = EventType::WindowExposed;
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

Event::Event(SDL_Event* event)
    : type_(eventTypeFromSDLEvent(event)) {
}

Event::Event(i32 etype)
    : type_(etype) {
}

i32 Event::type() const {
    return type_;
}

const Event::rtclock::time_point& Event::time() const {
    return tp_;
}

WindowEvent::WindowEvent(SDL_Event* ev)
    : Event(ev)
    , x_(0)
    , y_(0) {
    if (ev) {
        x_ = ev->window.data1;
        y_ = ev->window.data2;
    }
}

i32 WindowEvent::x() const {
    return x_;
}

i32 WindowEvent::y() const {
    return y_;
}

KeyboardEvent::KeyboardEvent(SDL_Event* event)
    : Event(event) {
    if (event) {
        key_ = Key(event->key.keysym.sym);
    }
}

const Key& KeyboardEvent::key() const {
    return key_;
}

KeyDownEvent::KeyDownEvent(SDL_Event* event, SDL_Event* text)
    : KeyboardEvent(event) {
    ch32 u32 = 0;
    if (text) {
        u32 = utf8ToUtf32(text->text.text)[0];
    }
    if (event) {
        key_ = Key(event->key.keysym, u32);
    }
}

MouseEvent::MouseEvent(SDL_Event* event)
    : Event(event)
    , buttons_(0) {
    buttons_ = SDL_GetMouseState(nullptr, nullptr);
}

uint32_t MouseEvent::buttons() const {
    return buttons_;
}

MouseMoveEvent::MouseMoveEvent(SDL_Event* event)
    : MouseEvent(event)
    , x_(0)
    , y_(0)
    , xrel_(0)
    , yrel_(0) {
    if (event) {
        x_ = event->motion.x;
        y_ = event->motion.y;
        xrel_ = event->motion.xrel;
        yrel_ = event->motion.yrel;
    }
}

i32 MouseMoveEvent::x() const {
    return x_;
}

i32 MouseMoveEvent::y() const {
    return y_;
}

i32 MouseMoveEvent::xrel() const {
    return x_;
}

i32 MouseMoveEvent::yrel() const {
    return y_;
}

MouseDownEvent::MouseDownEvent(SDL_Event* event)
    : MouseEvent(event)
    , button_(0) {
    if (event) {
        button_ = SDL_BUTTON(event->button.button);
    }
}

uint32_t MouseDownEvent::button() const {
    return button_;
}

MouseUpEvent::MouseUpEvent(SDL_Event* event)
    : MouseEvent(event)
    , button_(0) {
    if (event) {
        button_ = SDL_BUTTON(event->button.button);
    }
}

uint32_t MouseUpEvent::button() const {
    return button_;
}

MouseWheelEvent::MouseWheelEvent(SDL_Event* event)
    : Event(event)
    , dx_(0)
    , dy_(0) {
    if (event) {
        dx_ = event->wheel.x;
        dy_ = event->wheel.y;
    }
}

i32 MouseWheelEvent::dx() const {
    return dx_;
}

i32 MouseWheelEvent::dy() const {
    return dy_;
}

} // namespace events

} // namespace rterm
