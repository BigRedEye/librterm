#include <rterm/rterm.h>

int main() {
    namespace rt = rterm::terminal;

    rt::font::Collection fonts = rt::font::system()
        | rt::font::Filter{rt::font::family="DejaVu"}
        + rt::font::Font{rt::path="./12343245", rt::priority=12}
        + rt::font::system(rt::priority=0);

    rt::Config config;
    config
        .fonts(fonts)
        .width(80)
        .height(24)
        .points(14);

    rt::Terminal terminal{config};

    rt::Buffer buffer{
        rt::width = 80,
        rt::height = rt::dynamic,
    };
    auto id = terminal.attach(buffer);
    terminal.buffer(id).viewport({10, 10, 20, 20}).layer(123);

    terminal.on<rt::KeyDown>([&](rt::Key key) {
        std::cout << rt::serialize(key) << std::endl;
    });

    while (!terminal.closed()) {
        terminal.poll();
    }
}
