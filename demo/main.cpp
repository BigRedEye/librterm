#include "../include/term.h"
#include "../include/key.h"
#include <iostream>
#include <random>
#include <complex>

#define UNUSED(cond) (void)(cond)

int main(int argc, char **argv)
{
    UNUSED(argc);
    UNUSED(argv);

    term::Term terminal(80, 24);
    terminal.setFont("DejaVuSansMono.ttf", 18);
    terminal.setFgColor(term::Color(100, 255, 100));
    terminal.setFullscreen(false);
    bool fullscr = false;
    while (terminal.running()) {
        term::Key k = terminal.getKey();
        if (k.key() == term::F4 && k.mod() & term::ALT)
            return 42;
        if (k.key() == term::F1) {
            terminal.setFullscreen(!fullscr);
            fullscr = !fullscr;
        }
        else if (k.toChar())
            terminal.addChar(k.toChar());
        terminal.redraw();
    }
    return 0;
}
