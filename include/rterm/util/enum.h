#pragma once

#include <type_traits>
#include <functional>

namespace rterm::bitops {

enum BitOps {
    none = 0,
    opAnd = 1 << 0,
    opOr = 1 << 1,
    opXor = 1 << 2,
    opNot = 1 << 3,
    all = opAnd | opOr | opXor | opNot,
};

template<typename Enum, std::enable_if_t<std::is_enum_v<Enum>>* = nullptr>
inline constexpr bool generate_enum_bitops_v = BitOps::none;

namespace detail {

template<typename Enum, BitOps ops = BitOps::none>
using sfinae = std::enable_if_t<std::is_enum_v<Enum> && ((::rterm::bitops::generate_enum_bitops_v<Enum> & ops) == ops)>;

template<template<typename U> typename Func, typename T, typename ...Args>
inline constexpr T apply(T arg, Args ...args) {
    static_assert((std::is_enum_v<T> && ... && std::is_enum_v<Args>), "Invalid type");
    using U = std::underlying_type_t<T>;
    return static_cast<T>(Func<U>{}(static_cast<U>(arg), static_cast<U>(args)...));
}

} // namespace detail



} // namespace rterm

template<typename Enum, ::rterm::bitops::detail::sfinae<Enum, ::rterm::bitops::BitOps::opAnd>* = nullptr>
inline constexpr Enum operator&(Enum lhs, Enum rhs) {
    return ::rterm::bitops::detail::apply<std::bit_and>(lhs, rhs);
}

template<typename Enum, ::rterm::bitops::detail::sfinae<Enum, ::rterm::bitops::BitOps::opOr>* = nullptr>
inline constexpr Enum operator|(Enum lhs, Enum rhs) {
    return ::rterm::bitops::detail::apply<std::bit_or>(lhs, rhs);
}

template<typename Enum, ::rterm::bitops::detail::sfinae<Enum, ::rterm::bitops::BitOps::opXor>* = nullptr>
inline constexpr Enum operator^(Enum lhs, Enum rhs) {
    return ::rterm::bitops::detail::apply<std::bit_xor>(lhs, rhs);
}

template<typename Enum, ::rterm::bitops::detail::sfinae<Enum, ::rterm::bitops::BitOps::opNot>* = nullptr>
inline constexpr Enum operator~(Enum lhs) {
    return ::rterm::bitops::detail::apply<std::bit_not>(lhs);
}

template<typename Enum, ::rterm::bitops::detail::sfinae<Enum, ::rterm::bitops::BitOps::opAnd>* = nullptr>
inline constexpr Enum& operator&=(Enum& lhs, Enum rhs) {
    return lhs = (lhs & rhs);
}

template<typename Enum, ::rterm::bitops::detail::sfinae<Enum, ::rterm::bitops::BitOps::opOr>* = nullptr>
inline constexpr Enum& operator|=(Enum& lhs, Enum rhs) {
    return lhs = (lhs | rhs);
}

template<typename Enum, ::rterm::bitops::detail::sfinae<Enum, ::rterm::bitops::BitOps::opXor>* = nullptr>
inline constexpr Enum& operator^=(Enum& lhs, Enum rhs) {
    return lhs = (lhs ^ rhs);
}
