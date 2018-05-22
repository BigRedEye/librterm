#include "framerate_counter.h"

#include <numeric>


namespace rterm {

FrameRateCounter::FrameRateCounter(size_t framesToStore)
    : framesToStore_(framesToStore)
{
}

void FrameRateCounter::nextFrame(highResClock::time_point pt) {
    timePts_.push(pt);
    if (timePts_.size() > framesToStore_) {
        timePts_.pop();
    }
}

long double FrameRateCounter::getFps() const {
    std::chrono::duration<long double, std::ratio<1>> deltaTime =
        timePts_.back() - timePts_.front();
    if (deltaTime.count() < 1e-8) {
        return std::numeric_limits<long double>::infinity();
    }
    return static_cast<long double>(timePts_.size() - 1) / deltaTime.count();
}

} // namespace rterm
