#pragma once

#include <tl/expected.hpp>

namespace rterm {

template<typename T, typename E>
using Result = tl::expected<T, E>;
template<typename E>
using Error = tl::unexpected<E>;

template<typename T, typename E, typename L, typename R>
constexpr decltype(auto) match(const Result<T, E>& opt, L&& l, R&& r) {
    if (opt.has_value()) {
        return std::invoke(std::forward<L>(l), opt.value());
    } else {
        return std::invoke(std::forward<R>(r), opt.error());
    }
}

} // namespace rterm

