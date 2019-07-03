#include <rterm/terminal/terminal.h>

#include <rterm/window/window.h>

#include <iostream>

namespace rterm::terminal {

namespace {

class Impl {
public:
    Impl(const Options& opts) {
        std::cout << "size: " << opts.size()[0] << ' ' << opts.size()[1] << std::endl;
    }

private:
    window::Window window_;
};

} // namespace

Terminal::Terminal(const Options& opts)
    : StaticPimpl<Terminal>{opts} {
}

void Terminal::assertImplSize() {
    static_assert(sizeof(Impl) <= StaticPimpl::Size);
    static_assert(alignof(Impl) <= StaticPimpl::Size);
}

void Terminal::init(const Options& opts) {
    new (storage()) Impl(opts);
}

void Terminal::destroy() {
    impl<Impl>().~Impl();
}

} // namespace rterm::terminal
