#ifndef RTERM_EVENT_SYSTEM_H
#define RTERM_EVENT_SYSTEM_H

#include <thread>
#include <atomic>
#include <array>
#include <vector>
#include <functional>
#include <mutex>
#include <condition_variable>

#include "SDL2/SDL_events.h"
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
        std::lock_guard<std::mutex> lock(callbacksMutex_);
        callbacks_[eventType].emplace_back(callable);
    }
    bool quitRequested() const;
    void startPolling();
    void stopPolling();
    void join();
    int eventHandler(SDL_Event *ev);

    Key getKey();
    char_t getChar();
    Key getKey(const highResClock::time_point &until);
    char_t getChar(const highResClock::time_point &until);

private:
    std::thread eventPumpThread_;
    std::atomic_bool quitRequested_;
    std::array<std::vector<std::function<void(events::Event *)>>, events::EventType::COUNT> callbacks_;
    std::mutex callbacksMutex_;
    std::mutex waitMutex_;
    std::atomic_bool notified_; ///< to avoid spurious wakeups
    std::condition_variable condition_; ///< for blocking functions (getKey / getChar)
    Key pendingKey_;
};
}

/// @endcond

#endif // RTERM_EVENT_SYSTEM_H
