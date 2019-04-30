#pragma once

#include <rterm/util/defs.h>
#include <rterm/util/span.h>

#include <string_view>
#include <type_traits>

namespace rterm::murmur {

namespace detail {

inline constexpr u64 rotl64(u64 x, i8 r) {
    return (x << r) | (x >> (64 - r));
}

inline constexpr u64 fmix64(u64 k) {
    k ^= k >> 33;
    k *= 0xff51afd7ed558ccdull;
    k ^= k >> 33;
    k *= 0xc4ceb9fe1a85ec53ull;
    k ^= k >> 33;

    return k;
}

/* We need C++20 with std::bit_cast and constexpr std::copy */
template<typename Byte>
inline constexpr u64 getblock64(span<const Byte> s, size_t i) {
    static_assert(sizeof(Byte) == 1);

    span<const Byte> subspan = s.subspan(i * sizeof(u64), sizeof(u64));
    u64 result{0};
    for (size_t j = 0; j < sizeof(u64); ++j) {
        result |= u64{static_cast<u8>(subspan[j])} << (std::numeric_limits<u8>::digits * j);
    }
    return result;
}

/* constexpr implementation based on https://github.com/aappleby/smhasher/blob/master/src/MurmurHash3.cpp */
template<typename Byte>
constexpr std::pair<u64, u64> murmurhash(span<const Byte> data, const u32 seed) {
    static_assert(sizeof(Byte) == 1);

    const size_t nblocks = data.size() / 16;

    u64 h1 = seed;
    u64 h2 = seed;

    const u64 c1 = 0x87c37b91114253d5;
    const u64 c2 = 0x4cf5ad432745937f;

    /* canonical murmurhash implementations abuses strict aliasing rule; the following code will be optimized by compilers */
    for (size_t i = 0; i < nblocks; ++i) {
        u64 k1 = getblock64(data, i * 2 + 0);
        u64 k2 = getblock64(data, i * 2 + 1);

        k1 *= c1; k1 = rotl64(k1, 31); k1 *= c2; h1 ^= k1;

        h1 = rotl64(h1, 27); h1 += h2; h1 = h1 * 5 + 0x52dce729;

        k2 *= c2; k2 = rotl64(k2, 33); k2 *= c1; h2 ^= k2;

        h2 = rotl64(h2, 31); h2 += h1; h2 = h2 * 5 + 0x38495ab5;
    }

    span<const Byte> tail = data.subspan(nblocks * 16, data.size() - nblocks * 16);

    u64 k1 = 0;
    u64 k2 = 0;

    switch(data.size() & 0xf) {
        case 15: k2 ^= u64{static_cast<u8>(tail[14])} << 48;
        case 14: k2 ^= u64{static_cast<u8>(tail[13])} << 40;
        case 13: k2 ^= u64{static_cast<u8>(tail[12])} << 32;
        case 12: k2 ^= u64{static_cast<u8>(tail[11])} << 24;
        case 11: k2 ^= u64{static_cast<u8>(tail[10])} << 16;
        case 10: k2 ^= u64{static_cast<u8>(tail[ 9])} << 8;
        case  9: k2 ^= u64{static_cast<u8>(tail[ 8])} << 0;
            k2 *= c2;
            k2 = rotl64(k2, 33);
            k2 *= c1;
            h2 ^= k2;
        case  8: k1 ^= u64{static_cast<u8>(tail[ 7])} << 56;
        case  7: k1 ^= u64{static_cast<u8>(tail[ 6])} << 48;
        case  6: k1 ^= u64{static_cast<u8>(tail[ 5])} << 40;
        case  5: k1 ^= u64{static_cast<u8>(tail[ 4])} << 32;
        case  4: k1 ^= u64{static_cast<u8>(tail[ 3])} << 24;
        case  3: k1 ^= u64{static_cast<u8>(tail[ 2])} << 16;
        case  2: k1 ^= u64{static_cast<u8>(tail[ 1])} << 8;
        case  1: k1 ^= u64{static_cast<u8>(tail[ 0])} << 0;
            k1 *= c1;
            k1 = rotl64(k1, 31);
            k1 *= c2;
            h1 ^= k1;
    };

    h1 ^= data.size();
    h2 ^= data.size();

    h1 += h2;
    h2 += h1;

    h1 = fmix64(h1);
    h2 = fmix64(h2);

    h1 += h2;
    h2 += h1;

    return {h1, h2};
}

}

template<typename Byte>
constexpr inline size_t hash_impl(span<const Byte> data) {
    auto [lo, hi] = detail::murmurhash<Byte>(data, 0x728a4795);
    return lo ^ hi;
}

template<typename T, std::enable_if_t<std::has_unique_object_representations_v<T>>* = nullptr>
/* will be constexpr in C++20 */ inline size_t hash(const T& t) {
    return hash_impl<u8>(span<const u8>{reinterpret_cast<const u8*>(&t), sizeof(T)});
}

template<size_t Size>
constexpr inline size_t hash(const char (&arr)[Size]) {
    return hash_impl<char>(span{arr, Size});
}

template<size_t Size>
constexpr size_t size(const char (&)[Size]) {
    return Size;
}

#include "murmur_test.h"

} // namespace rterm::murmur
