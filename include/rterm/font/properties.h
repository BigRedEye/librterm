#pragma once

#include <rterm/util/variant.h>
#include <rterm/util/defs.h>

#include <string>

namespace rterm::font {

// Based on https://drafts.csswg.org/css-fonts-3
// 3.1 Family
enum class GenericFamily : u8 {
    serif = 1,
    sansSerif = 2,
    monospace = 3,
    cursive = 4,
    fantasy = 5,
};

using FamilyName = std::string;
using Family = Variant<GenericFamily, FamilyName>;

// 3.2 Weight
enum class Weight : u32 {
    thin = 100,
    extraLight = 200,
    light = 300,
    normal = 400,
    medium = 500,
    semiBold = 600,
    bold = 700,
    extraBold = 800,
    heavy = 900
};

// 3.3 Width
enum class Width : u8 {
    ultraCondensed = 1,
    extraCondensed = 2,
    condensed = 3,
    semiCondensed = 4,
    normal = 5,
    semiExpanded = 6,
    expanded = 7,
    extraExpanded = 8,
    ultraExpanded = 9,
};

struct Properties {
    Family family;
    std::string postscriptName;
    Weight weight;
    Width width;
    std::string style;
    bool monospace;
};

} // namespace rterm::font
