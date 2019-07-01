#pragma once

#include <rterm/util/static_pimpl.h>
#include <rterm/font/face.h>

namespace rterm {

enum class ShapingMode { quick, full };

class Shaper : StaticPimpl<Shaper> {
public:
    Shaper(ShapingMode mode);

    void shape(const Row& row, const font::Face& face);

private:
    friend class StaticPimpl<Shaper>;
    static void assertImplSize(size_t);
    void init();
    void destroy();
};

} // namespace rterm
