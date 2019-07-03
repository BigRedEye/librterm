#pragma once

#include <variant>
#include <tuple>

namespace rterm {

template<typename ...Args>
using Variant = std::variant<Args...>;
using Monostate = std::monostate;

namespace detail {

// https://en.cppreference.com/w/cpp/utility/variant/visit

template<typename ...T>
struct Matcher : T... {
    using T::operator()...;
};

template<typename ...T> Matcher(T...) -> Matcher<T...>;

template<typename Func>
struct MatcherChecker {
    template<typename ...Arg>
    inline static constexpr bool canBeInvoked = (std::is_invocable_v<Func, std::add_rvalue_reference<Arg>> || ... || false);
};

template<typename Arg>
struct ArgumentChecker {
    template<typename ...Func>
    inline static constexpr bool hasMatcher = (std::is_invocable_v<Func, std::add_rvalue_reference<Arg>> || ... || false);
};

} // namespace detail

template<typename ...Args, typename ...Func>
constexpr decltype(auto) match(Variant<Args...>&& variant, Func&& ...func) {
    static_assert((detail::MatcherChecker<Func>::template canBeInvoked<Args...> && ... && true), "Visitor will never be called");
    static_assert((detail::ArgumentChecker<Args>::template hasMatcher<Func...> && ... && true), "Argument does not have matching visitor");
    static_assert(sizeof...(Args) >= sizeof...(Func), "Found excess visitors");

    std::visit(detail::Matcher{ std::forward<Func>(func)... }, std::forward<decltype(variant)>(variant));
}

} // namespace rterm
