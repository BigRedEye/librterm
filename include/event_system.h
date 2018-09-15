#pragma once

#include "event.h"
#include "key.h"
#include "util.h"

#include <SDL_events.h>

#include <functional>
#include <vector>

namespace rterm {

class EventSystem {
public:
    using highResClock = std::chrono::high_resolution_clock;

    EventSystem();
    ~EventSystem();

    template<typename F>
    inline void registerCallback(i32 eventType, F&& callable) {
        callbacks_[eventType].emplace_back(callable);
    }

    bool quitRequested() const;
    void poll();
    i32 eventHandler(SDL_Event* ev);

    Key getKey();
    char_t getChar();
    Key getKey(const highResClock::time_point& until);
    char_t getChar(const highResClock::time_point& until);

private:
    bool quitRequested_;
    std::vector<std::vector<std::function<void(events::Event*)>>> callbacks_;
    Key pendingKey_;
};

} // namespace rterm

/// @endcond
