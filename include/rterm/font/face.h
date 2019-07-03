#pragma once

#include <rterm/font/font.h>
#include <rterm/font/handle.h>
#include <rterm/util/defs.h>
#include <rterm/util/static_pimpl.h>

namespace rterm::font {

class Face : StaticPimpl<Face> {
public:
    Face(const Handle& handle);

    bool monospace() const;

private:
    void init(const Handle& handle);
    void destroy();
    void assertImplSize();
};

} // namespace rterm
