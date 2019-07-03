#pragma once

#if RTERM_CONFIG_SOURCE_CORETEXT

#include <rterm/font/collection.h>

namespace rterm::font::source {

class CoreText {
public:
    Collection load();
};

} // namespace rterm::font::impl

#endif // RTERM_CONFIG_SOURCE_CORETEXT
