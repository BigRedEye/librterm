#include "term.h"
#include <iostream>
#include <random>
#include <complex>
#include "key.h"

#define UNUSED(cond) (void)(cond)

int main(int argc, char **argv)
{
    UNUSED(argc);
    UNUSED(argv);

    term::Term terminal(80, 24);
    terminal.setFont("DejaVuSansMono.ttf", 18);
    terminal.setFgColor(term::Color(0, 170, 0));
    terminal.setFullscreen();
    while (terminal.running()) {
        term::Key k = terminal.getKey();
        if (k.key() == term::F4 && k.mod() & term::ALT)
            return 42;
        else if (k.toChar())
            terminal.addChar(k.toChar());
        terminal.redraw();
    }
    return 0;
}
