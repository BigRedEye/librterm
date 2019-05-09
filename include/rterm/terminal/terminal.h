#pragma once

#include <rterm/terminal/options.h>
#include <rterm/util/defs.h>

#include <type_traits>

namespace rterm::terminal {

class Terminal {
public:
    explicit Terminal(const Options& opts);

    Terminal(const Terminal&) = delete;
    Terminal& operator=(const Terminal&) = delete;

    Terminal(Terminal&&) = default;
    Terminal& operator=(Terminal&&) = default;

    ~Terminal();

private:
    class Impl;

    Impl& impl();
    const Impl& impl() const;

private:
    /* static_assert'ed in the implementation */
    static constexpr size_t ImplSize{64};
    static constexpr size_t ImplAlign{alignof(std::max_align_t)};

    std::aligned_storage_t<ImplSize, ImplAlign> storage_;
};

} // namespace rterm::terminal
