#ifndef RTERM_SDL_LOCK_H
#define RTERM_SDL_LOCK_H

#include <mutex>

namespace rterm {

using mutex_t = std::recursive_mutex;

inline mutex_t& getSDLMutex() {
    static mutex_t mtx;
    return mtx;
}

inline std::unique_lock<mutex_t> acquireSDLMutex() {
    return std::unique_lock<mutex_t>(getSDLMutex());
}

#define SDL_CALL(function, ...) \
    getSDLMutex().lock(); \
    function(__VA_ARGS__); \
    getSDLMutex().unlock();
}

#endif // RTERM_SDL_LOCK_H
