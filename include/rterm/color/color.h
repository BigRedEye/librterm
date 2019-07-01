#pragma once

#include <rterm/util/defs.h>
#include <array>
#include <typeindex>

namespace rterm {

class alignas(u32) Color {
public:
    Color(u8 r = 0, u8 g = 0, u8 b = 0, u8 a = 0);

    u8 r() const;
    u8 g() const;
    u8 b() const;
    u8 a() const;
    u8& r();
    u8& g();
    u8& b();
    u8& a();

    u32 rgba() const; // a = rgba() & 0xff

private:
    std::array<u8, 4> rgba_;
};

bool operator==(const Color lhs, const Color rhs);
bool operator!=(const Color lhs, const Color rhs);

} // namespace rterm

namespace std {

template<>
struct hash<::rterm::Color> {
    std::size_t operator()(const ::rterm::Color color) const;
};

} // namespace std
