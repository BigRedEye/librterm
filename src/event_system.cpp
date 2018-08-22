#include "event_system.h"
#include "event.h"
#include "logger.h"

#include <memory>

namespace rterm {

EventSystem::EventSystem()
    : quitRequested_(false)
    , callbacks_(events::EventType::COUNT) {
    SDL_StartTextInput();
}

EventSystem::~EventSystem() {
    SDL_StopTextInput();
}

bool EventSystem::quitRequested() const {
    return quitRequested_;
}

void EventSystem::poll() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        eventHandler(&event);
    }
}

std::unique_ptr<events::Event> getKeyDownEvent(SDL_Event* ev) {
    events::Event* event = nullptr;
    if (ev->type == SDL_KEYDOWN) {
        SDL_PumpEvents();
        SDL_Event e;
        if (SDL_PeepEvents(&e, 1, SDL_GETEVENT, SDL_TEXTINPUT, SDL_TEXTINPUT) > 0) {
            event = new events::KeyDownEvent(ev, &e);
        } else {
            event = new events::KeyDownEvent(ev);
        }
    }
    return std::unique_ptr<events::Event>(event);
}

i32 EventSystem::eventHandler(SDL_Event* ev) {
    using namespace rterm::events;

    std::unique_ptr<Event> event;
    if (!ev) {
        return 1;
    }
    switch (ev->type) {
    case SDL_QUIT:
        event.reset(new QuitEvent());
        break;
    case SDL_WINDOWEVENT:
        switch (ev->window.event) {
        case SDL_WINDOWEVENT_SHOWN:
            event.reset(new WindowShownEvent(ev));
            break;
        case SDL_WINDOWEVENT_HIDDEN:
            event.reset(new WindowHiddenEvent(ev));
            break;
        case SDL_WINDOWEVENT_RESIZED:
            event.reset(new WindowResizedEvent(ev));
            break;
        case SDL_WINDOWEVENT_MOVED:
            event.reset(new WindowMovedEvent(ev));
            break;
        case SDL_WINDOWEVENT_EXPOSED:
            event.reset(new WindowExposedEvent(ev));
            break;
        default:
            break;
        }
        break;
    case SDL_SYSWMEVENT:
        event.reset(new SystemEvent(ev));
        break;
    case SDL_KEYDOWN:
        event = getKeyDownEvent(ev);
        break;
    case SDL_KEYUP:
        event.reset(new KeyUpEvent(ev));
        break;
    case SDL_MOUSEMOTION:
        event.reset(new MouseMoveEvent(ev));
        break;
    case SDL_MOUSEBUTTONDOWN:
        event.reset(new MouseDownEvent(ev));
        break;
    case SDL_MOUSEBUTTONUP:
        event.reset(new MouseUpEvent(ev));
        break;
    case SDL_MOUSEWHEEL:
        event.reset(new MouseWheelEvent(ev));
        break;
    default:
        break;
    }
    if (event) {
        for (auto&& callback : callbacks_[event->type()]) {
            callback(event.get());
        }
    }
    if (event && event->type() == EventType::KeyDown) {
        events::KeyDownEvent* keyev = static_cast<events::KeyDownEvent*>(event.get());
        pendingKey_ = keyev->key();
    }
    return 1;
}

Key EventSystem::getKey() {
    pendingKey_ = Key();
    while (!quitRequested_ && !pendingKey_) {
        poll();
    }
    return pendingKey_;
}

char_t EventSystem::getChar() {
    while (true) {
        Key key = getKey();
        if (key.toChar()) {
            return key.toChar();
        }
        if (quitRequested_) {
            return 0;
        }
    }
}

Key EventSystem::getKey(const highResClock::time_point& until) {
    while (!quitRequested_ && !pendingKey_ && highResClock::now() < until) {
        poll();
    }
    return pendingKey_;
}

char_t EventSystem::getChar(const highResClock::time_point& until) {
    while (highResClock::now() < until) {
        Key key = getKey(until);
        if (key.toChar()) {
            return key.toChar();
        }
        if (quitRequested_) {
            break;
        }
    }
    return 0;
}

} // namespace rterm
