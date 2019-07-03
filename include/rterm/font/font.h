#pragma once

#include <rterm/font/handle.h>
#include <rterm/font/properties.h>

#include <rterm/util/defs.h>
#include <rterm/util/named_args.h>
#include <rterm/util/fs.h>

namespace rterm::font {

struct Font {
    Handle handle;
    Properties properties;
};

} // namespace rterm
