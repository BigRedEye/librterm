#ifndef RTERM_FPS_COUNTER_H
#define RTERM_FPS_COUNTER_H

#include <chrono>
#include <queue>

namespace rterm {

using highResClock = std::chrono::high_resolution_clock;

class FrameRateCounter {
public:
    static constexpr size_t defaultFramesToStore = 20;

    FrameRateCounter(size_t framesToStore = defaultFramesToStore);
    
    void nextFrame(highResClock::time_point pt);
    long double getFps() const;
private:
    std::queue<highResClock::time_point> timePts_;
    size_t framesToStore_;
};
}

#endif // RTERM_FPS_COUNTER_H
