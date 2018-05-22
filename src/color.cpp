#include "color.h"

namespace rterm {
Color::Color(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a)
    : r_(_r)
    , g_(_g)
    , b_(_b)
    , a_(_a)
{
}

bool Color::operator==(const Color& other) const {
    return r() == other.r() &&
           g() == other.g() &&
           b() == other.b() &&
           a() == other.a();
}

bool Color::operator!=(const Color& other) const {
    return !(*this == other);
}
}
