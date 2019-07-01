#pragma once

#include <functional>
#include <type_traits>

namespace rterm::bitops {

enum BitOps {
    none = 0,
    opAnd = 1 << 0,
    opOr = 1 << 1,
    opXor = 1 << 2,
    opNot = 1 << 3,
    opLogicalNot = 1 << 4,
    all = opAnd | opOr | opXor | opNot | opLogicalNot,
};

template<typename Enum>
inline constexpr bool is_scoped_enum() {
    if constexpr (std::is_enum_v<Enum>) {
        return !std::is_convertible_v<Enum, std::underlying_type_t<Enum>>;
    } else {
        return false;
    }
}

template<typename Enum>
inline constexpr bool is_scoped_enum_v = is_scoped_enum<Enum>();

static_assert(!is_scoped_enum_v<BitOps>);

template<typename Enum>
struct generate_enum_bitops {
    static constexpr BitOps value = BitOps::none;
};

template<typename Enum>
inline constexpr BitOps generate_enum_bitops_v = generate_enum_bitops<Enum>::value;

namespace detail {

template<template<typename U> typename Func, typename T, typename... Args>
inline constexpr T apply(T arg, Args... args) {
    static_assert((is_scoped_enum_v<T> && ... && is_scoped_enum_v<Args>), "Invalid type");
    using U = std::underlying_type_t<T>;
    return static_cast<T>(Func<U>{}(static_cast<U>(arg), static_cast<U>(args)...));
}

} // namespace detail

} // namespace rterm::bitops

template<typename Enum>
inline constexpr bool should_generate(::rterm::bitops::BitOps operation) {
    using BitOps = ::rterm::bitops::BitOps;
    return static_cast<std::underlying_type_t<BitOps>>(operation) &
           static_cast<std::underlying_type_t<BitOps>>(::rterm::bitops::generate_enum_bitops_v<Enum>);
}

template<
    typename Enum,
    ::rterm::bitops::BitOps ops = ::rterm::bitops::BitOps::opAnd,
    std::enable_if_t<::rterm::bitops::is_scoped_enum_v<Enum> && should_generate<Enum>(ops)>* = nullptr>
inline constexpr Enum operator&(Enum lhs, Enum rhs) {
    return ::rterm::bitops::detail::apply<std::bit_and>(lhs, rhs);
}

template<
    typename Enum,
    ::rterm::bitops::BitOps ops = ::rterm::bitops::BitOps::opOr,
    std::enable_if_t<::rterm::bitops::is_scoped_enum_v<Enum> && should_generate<Enum>(ops)>* = nullptr>
inline constexpr Enum operator|(Enum lhs, Enum rhs) {
    return ::rterm::bitops::detail::apply<std::bit_or>(lhs, rhs);
}

template<
    typename Enum,
    ::rterm::bitops::BitOps ops = ::rterm::bitops::BitOps::opXor,
    std::enable_if_t<::rterm::bitops::is_scoped_enum_v<Enum> && should_generate<Enum>(ops)>* = nullptr>
inline constexpr Enum operator^(Enum lhs, Enum rhs) {
    return ::rterm::bitops::detail::apply<std::bit_xor>(lhs, rhs);
}

template<
    typename Enum,
    ::rterm::bitops::BitOps ops = ::rterm::bitops::BitOps::opNot,
    std::enable_if_t<::rterm::bitops::is_scoped_enum_v<Enum> && should_generate<Enum>(ops)>* = nullptr>
inline constexpr Enum operator~(Enum lhs) {
    return ::rterm::bitops::detail::apply<std::bit_not>(lhs);
}

template<
    typename Enum,
    ::rterm::bitops::BitOps ops = ::rterm::bitops::BitOps::opLogicalNot,
    std::enable_if_t<::rterm::bitops::is_scoped_enum_v<Enum> && should_generate<Enum>(ops)>* = nullptr>
inline constexpr bool operator!(Enum e) {
    return !static_cast<std::underlying_type_t<Enum>>(e);
}

template<
    typename Enum,
    ::rterm::bitops::BitOps ops = ::rterm::bitops::BitOps::opAnd,
    std::enable_if_t<::rterm::bitops::is_scoped_enum_v<Enum> && should_generate<Enum>(ops)>* = nullptr>
inline constexpr Enum& operator&=(Enum& lhs, Enum rhs) {
    return lhs = (lhs & rhs);
}

template<
    typename Enum,
    ::rterm::bitops::BitOps ops = ::rterm::bitops::BitOps::opOr,
    std::enable_if_t<::rterm::bitops::is_scoped_enum_v<Enum> && should_generate<Enum>(ops)>* = nullptr>
inline constexpr Enum& operator|=(Enum& lhs, Enum rhs) {
    return lhs = (lhs | rhs);
}

template<
    typename Enum,
    ::rterm::bitops::BitOps ops = ::rterm::bitops::BitOps::opXor,
    std::enable_if_t<::rterm::bitops::is_scoped_enum_v<Enum> && should_generate<Enum>(ops)>* = nullptr>
inline constexpr Enum& operator^=(Enum& lhs, Enum rhs) {
    return lhs = (lhs ^ rhs);
}

#define RTERM_GENERATE_ENUM_BITWISE_OPERATORS(Enum)                                                       \
    static_assert(::rterm::bitops::is_scoped_enum_v<Enum>, "Scoped enum required for bitwise operators"); \
    namespace detail_bitops = ::rterm::bitops;                                                            \
    template<>                                                                                            \
    struct detail_bitops::generate_enum_bitops<Enum> {                                                    \
        static constexpr BitOps value = BitOps::all;                                                      \
    }

namespace rterm {

template<typename Enum, std::enable_if_t<std::is_enum_v<Enum>>* = nullptr>
inline constexpr auto eindex(Enum e) {
    return static_cast<std::underlying_type_t<Enum>>(e);
}

} // namespace rterm
