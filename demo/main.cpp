#include "../include/term.h"
#include "../include/key.h"
#include <iostream>
#include <random>
#include <complex>
#include <cstring>

#define UNUSED(cond) (void)(cond)

int main(int argc, char **argv)
{
    UNUSED(argc);
    UNUSED(argv);

    term::Term terminal(80, 24);
    terminal.setFont("DejaVuSansMono.ttf", 18);
    terminal.setFgColor(term::Color(100, 255, 100));
    terminal.setFullscreen(false);
    if (argc > 1 && !strcmp(argv[1], "--test")) {
        for (int i = 0; i < 1000; ++i)
            terminal.setChar(rand() % terminal.cols(), rand() % terminal.rows(), 'a' + rand() % 26);
    } else {
        bool fullscr = false;
        while(terminal.running()) {
            term::Key k = terminal.getKey();
            if (k.key() == term::F4 && k.mod() & term::ALT)
                return 0;
            if (k.key() == term::F1) {
                terminal.setFullscreen(!fullscr);
                fullscr = !fullscr;
            }
            else if (k.toChar())
                terminal.addChar(k.toChar());
            terminal.redraw();
        }
    }
    return 0;
}
