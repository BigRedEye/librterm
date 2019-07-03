#pragma once

#if RTERM_CONFIG_SOURCE_DIRECTWRITE

#include <rterm/font/collection.h>

namespace rterm::font::source {

class DirectWrite {
public:
    Collection load();
};

} // namespace rterm::font::impl

#endif // RTERM_CONFIG_SOURCE_DIRECTWRITE
