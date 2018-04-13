#include <rterm/term.h>
#include <rterm/key.h>
#include <rterm/mouse.h>
#include <rterm/logger.h>

#include <iostream>
#include <random>
#include <cstring>
#include <chrono>

#define UNUSED(cond) (void)(cond)

int main(int argc, char **argv) {
    /* on Windows SDL2 redeclares main with custom main function,
       so we need argc and argv */
    UNUSED(argc);
    UNUSED(argv);
    /* creates empty terminal with 80 columns and 24 rows */
    rterm::Term terminal(80, 24);
    terminal.setTitle("rterm demo");
    /* loads TrueType font */
    int fontSize = 18;
    terminal.setFont("../fonts/ttf/DejaVuSansMono.ttf", fontSize);
    /* sets foreground color to green (#00FF00)*/
    terminal.setFgColor(rterm::Color(0x00, 0xff, 0x00));
    /* sets callback for keypress */
    terminal.onKeyDown([&](rterm::events::KeyDownEvent ev) {
        /* if key has visual representation...*/
        if (ev.key().toChar())
            /* add character to terminal */
            terminal.addChar(ev.key().toChar());
        /* and draw everything on screen */
        terminal.redraw();
    });
    /* while terminal is not closed... */
    while (terminal.isRunning());
    return 0;
}
