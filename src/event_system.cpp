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
    notified_ = false;
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
        condition_.notify_all();
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
    if (!ev)
        return 1;
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
        std::lock_guard<std::mutex> lock(callbacksMutex_);
        for (auto &&callback : callbacks_[event->type()])
            callback(event.get());
    }
    if (event && event->type() == EventType::KeyDown) {
        events::KeyDownEvent *keyev = static_cast<events::KeyDownEvent*>(event.get());
        pendingKey_ = keyev->key();
        notified_ = true;
        condition_.notify_all();
    }
    return 1;
}

Key EventSystem::getKey() {
    std::unique_lock<std::mutex> lock(waitMutex_);
    notified_ = false;
    while (!quitRequested_ && !notified_)
        condition_.wait(lock);
    if (!notified_)
        return Key();
    notified_ = false;
    Key res = pendingKey_;
    return res;
}

char_t EventSystem::getChar() {
    while (true) {
        Key key = getKey();
        if (key.toChar())
            return key.toChar();
    }
}

Key EventSystem::getKey(const highResClock::time_point &until) {
    std::unique_lock<std::mutex> lock(waitMutex_);
    notified_ = false;
    while (!quitRequested_ && !notified_)
        condition_.wait_until(lock, until);
    if (!notified_)
        return Key();
    notified_ = false;
    Key res = pendingKey_;
    return res;
}

char_t EventSystem::getChar(const highResClock::time_point &until) {
    while (highResClock::now() < until) {
        Key key = getKey(until);
        if (key.toChar())
            return key.toChar();
    }
    return 0;
}

}