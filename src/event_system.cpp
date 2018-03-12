#include "event_system.h"

namespace rterm {
EventSystem::EventSystem() {
    eventPumpThread_ = std::thread([this](){
        while (true) {
            SDL_PumpEvents();
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    });
}
}
