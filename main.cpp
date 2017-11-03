#include "term.h"
#include <iostream>

#define UNUSED(cond) (void)(cond)

int main(int argc, char **argv)
{
    UNUSED(argc);
    UNUSED(argv);

    term::Term terminal(40, 20);
    while (terminal.running()) {
        terminal.addChar(terminal.getChar());
        terminal.redraw();
    }
    return 0;
}
