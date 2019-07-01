#include <rterm/color/color.h>

namespace rterm {

Color::Color(u8 r, u8 g, u8 b, u8 a)
    : rgba_{{r, g, b, a}}
{}

bool operator==(const Color lhs, const Color rhs) {
    return lhs.rgba() == rhs.rgba();
}

bool operator!=(const Color lhs, const Color rhs) {
    return !(lhs == rhs);
}

} // namespace rterm

namespace std {

std::size_t hash<::rterm::Color>::operator()(const ::rterm::Color color) const {
    return std::hash<rterm::u32>{}(color.rgba());
}

}
