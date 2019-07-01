#pragma once

#include <rterm/util/static_pimpl.h>

namespace rterm::font {

class Font;
struct FT_Face;

class Face : StaticPimpl<Face> {
public:
    Face(const Font& font);

    const FT_Face& raw() const;

};

} // namespace rterm
