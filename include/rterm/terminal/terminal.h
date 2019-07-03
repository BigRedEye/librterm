#pragma once

#include <rterm/terminal/options.h>
#include <rterm/buffer/buffer.h>
#include <rterm/util/defs.h>
#include <rterm/util/static_pimpl.h>

namespace rterm::terminal {

using BufferId = u64;

class Terminal : public StaticPimpl<Terminal, 64> {
public:
    using Base = StaticPimpl<Terminal, 64>;
    explicit Terminal(const Options& opts);

    BufferId attach();
    Buffer& buffer(BufferId id);

private:
    friend class StaticPimpl<Terminal>;
    static void assertImplSize();
    void init(const Options& opts);
    void destroy();
};

} // namespace rterm::terminal
