#ifndef RTERM_SDL_LOCK_H
#define RTERM_SDL_LOCK_H

#include <mutex>
#include <type_traits>

namespace rterm {

using mutex_t = std::recursive_mutex;

inline mutex_t& getSDLMutex() {
    static mutex_t mtx;
    return mtx;
}

inline std::unique_lock<mutex_t> acquireSDLMutex() {
    return std::unique_lock<mutex_t>(getSDLMutex());
}

template<typename F, typename ...Args>
typename std::result_of<F(Args...)>::type SDL_Call(F&& f, Args&& ...args) {
    auto lock = acquireSDLMutex();
    return f(std::forward<Args>(args)...);
}
}

#endif // RTERM_SDL_LOCK_H
