#include <rterm/rterm.h>

int main() {
    namespace rt = rterm;

    rt::font::Collection fonts = rt::font::system()
        | rt::font::filter(rt::font::family="DejaVu")
        + rt::font::from(rt::path="./12343245", rt::priority=12)
        + rt::font::system(rt::priority=0);

    rt::font::Collection fonts = rt::font::system()
        .filter([](const rt::font::Properties& props) {
            return props.family == "DejaVu";
        })
        + rt::font::from(rt::path="./12343245", rt::priority=12)
        + rt::font::system(rt::priority=0);
}

