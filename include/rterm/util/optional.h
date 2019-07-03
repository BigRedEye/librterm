#pragma once

#include <tl/optional.hpp>

namespace rterm {

template<typename T>
using Optional = tl::optional<T>;
auto&& Empty = tl::nullopt;

template<typename T, typename L, typename R>
constexpr decltype(auto) match(const Optional<T>& opt, L&& l, R&& r) {
    if (opt.has_value()) {
        return std::invoke(std::forward<L>(l), opt.value());
    } else {
        return std::invoke(std::forward<R>(r));
    }
}

} // namespace rterm
