#include "event_system.h"
#include "sdl_lock.h"
#include "event.h"

#include "logger.h"

namespace rterm {
EventSystem::EventSystem() :
    quitRequested_(false) {
}

EventSystem::~EventSystem() {
    stopPolling();
    join();
}

bool EventSystem::quitRequested() const {
    return quitRequested_.load();
}

void EventSystem::startPolling() {
    quitRequested_ = false;
    eventPumpThread_ = std::thread([this](){
        while (!this->quitRequested_) {
            SDL_Event event;
            auto start = std::chrono::high_resolution_clock::now();
            {
                auto lock = acquireSDLMutex();
                while (SDL_PollEvent(&event))
                    eventHandler(&event);
            }
            std::this_thread::sleep_until(start + std::chrono::milliseconds(1));
        }
    });
}

void EventSystem::stopPolling() {
    quitRequested_ = true;
}

void EventSystem::join() {
    if (eventPumpThread_.joinable())
        eventPumpThread_.join();
}

std::unique_ptr<events::Event> getKeyDownEvent(SDL_Event *ev) {
    events::Event *event = nullptr;
    if (ev->type == SDL_KEYDOWN) {
        std::string str;
        SDL_Event e;
        if (SDL_PeepEvents(&e, 1, SDL_GETEVENT, SDL_TEXTINPUT, SDL_TEXTINPUT) > 0)
            event = new events::KeyDownEvent(ev, &e);
        else
            event = new events::KeyDownEvent(ev);
    }
    return std::unique_ptr<events::Event>(event);
}

int EventSystem::eventHandler(SDL_Event *ev) {
    using namespace rterm::events;

    std::unique_ptr<Event> event;
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
        default:
            break;
        }
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
        std::lock_guard<std::mutex> lock(callbacksMutex_);
        for (auto &&callback : callbacks_[event->type()])
            callback(event.get());
    }
    return 1;
}
}
