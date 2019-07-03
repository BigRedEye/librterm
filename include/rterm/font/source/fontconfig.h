#pragma once

#if RTERM_CONFIG_SOURCE_FONTCONFIG

#include <rterm/font/collection.h>

namespace rterm::font::source {

class FontConfig {
public:
    Collection load();
};

} // namespace rterm::font::impl

#endif // RTERM_CONFIG_SOURCE_FONTCONFIG
