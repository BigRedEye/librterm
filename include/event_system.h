#ifndef RTERM_EVENT_SYSTEM_H
#define RTERM_EVENT_SYSTEM_H

#include <array>
#include <vector>
#include <functional>

#include <SDL2/SDL_events.h>
#include "event.h"
#include "key.h"

namespace rterm {
class EventSystem {
public:
    using highResClock = std::chrono::high_resolution_clock;

    EventSystem();
    ~EventSystem();

    template<typename F>
    void registerCallback(int eventType, F &&callable) {
        callbacks_[eventType].emplace_back(callable);
    }
    bool quitRequested() const;
    void poll();
    int eventHandler(SDL_Event *ev);

    Key getKey();
    char_t getChar();
    Key getKey(const highResClock::time_point &until);
    char_t getChar(const highResClock::time_point &until);

private:
    bool quitRequested_;
    std::array<std::vector<std::function<void(events::Event *)>>, events::EventType::COUNT> callbacks_;
    Key pendingKey_;
};
}

/// @endcond

#endif // RTERM_EVENT_SYSTEM_H
