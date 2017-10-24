#include "term.h"
#include <iostream>

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    term::Term terminal(40, 30);
    terminal.redraw();
    while (terminal.running()) {
        terminal.addChar(terminal.getChar());
        terminal.redraw();
        SDL_Delay(1);
    }
    return 0;
}
