#pragma once

#include <type_traits>
#include <utility>
#include <functional>

namespace rterm {

template<typename F>
class ScopedGuard {
public:
    ScopedGuard(F&& f)
        : f_{std::move(f)} {
    }

    ScopedGuard(const F& f)
        : f_{f} {
    }

    ScopedGuard(const ScopedGuard&) = delete;
    ScopedGuard& operator=(const ScopedGuard&) = delete;

    ScopedGuard(ScopedGuard&&) = delete;
    ScopedGuard& operator=(ScopedGuard&&) = delete;

    ~ScopedGuard() noexcept {
        try {
            std::invoke(std::move(f_));
        } catch (...) {
            /* ¯\_(ツ)_/¯ */
        }
    }

private:
    F f_;
};

} // namespace rterm
