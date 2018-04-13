#include <rterm/term.h>

int main(int argc, char **argv) {
    rterm::Term terminal(80, 24);
    terminal.setFont("../fonts/ttf/DejaVuSansMono.ttf", 18);
    terminal.setFgColor(rterm::Color(0x00, 0xff, 0x00));
    terminal.onKeyDown([&](rterm::events::KeyDownEvent ev) {
        if (ev.key().toChar())
            terminal.addChar(ev.key().toChar());
        terminal.redraw();
    });
    while (terminal.isRunning())
        terminal.poll();
    return 0;
}
