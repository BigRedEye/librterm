#include "event_system.h"
#include "sdl_lock.h"
#include "event.h"

namespace rterm {
EventSystem::EventSystem() :
    quitRequested_(false) {
}

EventSystem::~EventSystem() {
    stopPolling();
    if (eventPumpThread_.joinable())
        eventPumpThread_.join();
}

bool EventSystem::quitRequested() const {
    return quitRequested_.load();
}

void EventSystem::startPolling() {
    SDL_CALL(SDL_AddEventWatch, eventFilter, this);
    eventPumpThread_ = std::thread([this](){
        while (!this->quitRequested_) {
            SDL_CALL(SDL_PumpEvents);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    });
}

void EventSystem::stopPolling() {
    quitRequested_ = true;
}

int EventSystem::eventHandler(SDL_Event *ev) {
    Event event(ev);
    for (const auto &callback : callbacks_[event.type()])
        callback(&event);
    return 1;
}
}

int eventFilter(void *data, SDL_Event *ev) {
    using namespace rterm;
    EventSystem *ptr = reinterpret_cast<EventSystem*>(data);
    return ptr->eventHandler(ev);
}
