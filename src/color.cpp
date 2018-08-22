#include "color.h"

namespace rterm {

Color::Color(ui8 _r, ui8 _g, ui8 _b, ui8 _a)
    : r_(_r)
    , g_(_g)
    , b_(_b)
    , a_(_a) {
}

bool Color::operator==(const Color& other) const {
    return r() == other.r() && g() == other.g() && b() == other.b() &&
           a() == other.a();
}

bool Color::operator!=(const Color& other) const {
    return !(*this == other);
}

} // namespace rterm
