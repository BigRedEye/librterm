#include <rterm/key.h>
#include <rterm/logger.h>
#include <rterm/mouse.h>
#include <rterm/term.h>
#include <rterm/error.h>

#define UNUSED(cond) (void)(cond)

int main(int argc, char **argv) {
    /* on Windows SDL2 redefines main with custom SDL_main function,
     * so we need argc and argv 
     */
    UNUSED(argc);
    UNUSED(argv);
    /* rterm checks every SDL2 call, so: */
    try {
        /* create empty terminal with 80 columns and 24 rows */
        rterm::Term terminal(80, 24);
        /* set window title */
        terminal.setTitle("rterm example");
        /* load TrueType font */
        int fontSize = 18;
        terminal.setFont("../fonts/ttf/DejaVuSansMono.ttf", fontSize);
        /* set foreground color to green (#00FF00) */
        terminal.setFgColor(rterm::Color(0x00, 0xff, 0x00));
        /* set callback for keypress */
        terminal.onKeyDown([&](rterm::events::KeyDownEvent ev) {
            /* if key has visual representation... */
            if (ev.key().toChar()) {
                /* add character to terminal */
                terminal.addChar(ev.key().toChar());
                /* and draw everything on screen */
                terminal.redraw();
            }
        });
        /* while terminal is not closed... */
        while (terminal.isRunning()) {
            /* poll events */
            terminal.poll();
        }
        return 0;
    } catch (const rterm::Exception& e) {
        rterm::Logger() << e.what();
    }
}
