#include <rterm/terminal/terminal.h>

#include <rterm/window/window.h>

namespace rterm::terminal {

class Terminal::Impl {
public:
    Impl(const Options& opts) {
    }

private:
    window::Window window_;
};

Terminal::Terminal(const Options& opts) {
    static_assert(sizeof(Impl) <= Terminal::ImplSize);
    static_assert(alignof(Impl) <= Terminal::ImplAlign);

    new (&storage_) Impl(opts);
}

} // namespace rterm::terminal
