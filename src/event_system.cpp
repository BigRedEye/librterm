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
    if (eventPumpThread_.joinable())
        eventPumpThread_.join();
}

bool EventSystem::quitRequested() const {
    return quitRequested_.load();
}

void EventSystem::startPolling() {
    // SDL_CALL(SDL_AddEventWatch, eventFilter, this);
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

int EventSystem::eventHandler(SDL_Event *ev) {
    Event event(ev);
    if (ev->type == SDL_TEXTINPUT) {
        std::string str;
        if (ev->text.windowID == 123545)
            str = "pushed";
        else
            str = std::to_string(ev->text.windowID);
        Logger() << "TextInput " << str;
    }
    if (ev->type == SDL_KEYDOWN) {
        std::string str;
        SDL_Event e;
        if (SDL_PeepEvents(&e, 1, SDL_GETEVENT, SDL_TEXTINPUT, SDL_TEXTINPUT) > 0)
            str = " with TextEditing";
        Logger() << "KeyDown" + str;
    }
    if (ev->type == SDL_TEXTEDITING)
        Logger() << "TextEditing";
    if (ev->type == SDL_KEYUP)
        Logger() << "KeyUp";

    std::lock_guard<std::mutex> lock(callbacksMutex_);
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
