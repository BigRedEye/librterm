#ifndef RTERM_EVENT_SYSTEM_H
#define RTERM_EVENT_SYSTEM_H

#include <thread>
#include <atomic>
#include <array>
#include <vector>
#include <functional>

#include "SDL2/SDL_events.h"
#include "event.h"

namespace rterm {
class EventSystem {
public:
    EventSystem();
    ~EventSystem();

    template<typename F>
    void registerCallback(int eventType, F &&callable) {
        callbacks_[eventType].emplace_back(callable);
    }
    bool quitRequested() const;
    void startPolling();
    void stopPolling();
    int eventHandler(SDL_Event *ev);

private:
    std::thread eventPumpThread_;
    std::atomic_bool quitRequested_;
    std::array<std::vector<std::function<void(Event *)>>, EventType::COUNT> callbacks_;
};
}

/// @cond INTERNAL
/**
 * @brief SDL event filter
 * @param data user data
 * @param ev next event
 * @return 1
 */
int eventFilter(void *data, SDL_Event *ev);

/// @endcond

#endif // RTERM_EVENT_SYSTEM_H
