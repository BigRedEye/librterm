#pragma once

#include <rterm/terminal/options.h>
#include <rterm/util/defs.h>

#include <type_traits>

namespace rterm::terminal {

class Terminal {
public:
    explicit Terminal(const Options& opts = Options{});

    Terminal(const Terminal&) = delete;
    Terminal& operator=(const Terminal&) = delete;

    Terminal(Terminal&&) = default;
    Terminal& operator=(Terminal&&) = default;

    ~Terminal();

private:
    static constexpr size_t ImplSize{64}; ///< static_assert'ed in the implementation

    std::aligned_storage<ImplSize, alignof(std::max_align_t)> impl_;
};

} // namespace rterm::terminal
