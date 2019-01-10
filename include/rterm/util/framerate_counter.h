#pragma once

#include "rterm/util/util.h"

#include <chrono>
#include <queue>

namespace rterm {

/// @cond INTERNAL

using highResClock = std::chrono::high_resolution_clock;

/**
 * @brief Helper class for fps calculating
 */
class FrameRateCounter {
public:
    static constexpr ui32 defaultFramesToStore = 20;

    /**
     * @brief Constructs FrameRateCoutner
     * @param framesToStore maximum frames to use in calculation of average fps
     */
    FrameRateCounter(ui32 framesToStore = defaultFramesToStore);

    /**
     * @brief Add next frame timepoint
     * @param pt timepoint
     */
    void nextFrame(highResClock::time_point pt);

    /**
     * @brief Get average fps during last framesToStore_ frames
     * @return average fps
     */
    long double getFps() const;

private:
    std::queue<highResClock::time_point> timePts_; ///< queue of timepoints
    ui32 framesToStore_;                           ///< maximum number of frames to store
};

/// @endcond

} // namespace rterm
