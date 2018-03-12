#ifndef RTERM_EVENT_SYSTEM_H
#define RTERM_EVENT_SYSTEM_H

#include <thread>

#include "SDL2/SDL_events.h"

namespace rterm {
class EventSystem {
public:
    EventSystem();

private:
    std::thread eventPumpThread_;
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
