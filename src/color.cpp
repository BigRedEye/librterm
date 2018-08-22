#include "color.h"

namespace rterm {

Color::Color(ui8 _r, ui8 _g, ui8 _b, ui8 _a)
    : r_(_r)
    , g_(_g)
    , b_(_b)
    , a_(_a) {
}

bool Color::operator==(const Color& other) const {
    return r() == other.r() && g() == other.g() && b() == other.b() && a() == other.a();
}

bool Color::operator!=(const Color& other) const {
    return !(*this == other);
}

ui32 Color::rgba() const {
    return (ui32{r()} << 24) | (ui32{g()} << 16) | (ui32{b()} << 8) | (ui32{a()});
}

const Color Color::Black  {0x00, 0x00, 0x00, 0xff};
const Color Color::Red    {0xff, 0x00, 0x00, 0xff};
const Color Color::Green  {0x00, 0xff, 0x00, 0xff};
const Color Color::Yellow {0xff, 0xff, 0x00, 0xff};
const Color Color::Blue   {0x00, 0x00, 0xff, 0xff};
const Color Color::Magenta{0xff, 0x00, 0xff, 0xff};
const Color Color::Cyan   {0x00, 0xff, 0xff, 0xff};
const Color Color::White  {0xff, 0xff, 0xff, 0xff};

} // namespace rterm
