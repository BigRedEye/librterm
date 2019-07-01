#include <rterm/rterm.h>

int main() {
    namespace rt = rterm::terminal;

    rt::font::Set fonts = rt::font::system()
        | rt::font::Filter{rt::font::family="DejaVu"}
        + rt::font::Font{rt::path="./12343245", rt::priority=12}
        + rt::font::system(rt::priority=0);

    rt::Terminal terminal({
        rt::width = 80,
        rt::height = 24,
        rt::fonts = fonts,
        rt::points = 14,
    });

    rt::Buffer buffer{
        rt::width = 100,
        rt::height = 100,
        rt::scrollable,
    };
    auto id = terminal.attach(buffer);
    terminal.buffer(id).viewport({1, 1, -1, -1});
    terminal.buffer(id).layer(123);

    terminal.on<rt::KeyDown>([&](rt::Key key) {
        std::cout << rt::serialize(key) << std::endl;
    });

    while (!terminal.closed()) {
        terminal.poll();
    }
}
