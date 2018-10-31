#include <key.h>
#include <logger.h>
#include <mouse.h>
#include <term.h>
#include <error.h>

#include <chrono>

#define UNUSED(cond) (void)(cond)

int main(int argc, char *argv[]) {
    /* on Windows SDL2 redefines main with custom SDL_main function,
     * so we need argc and argv 
     */
    UNUSED(argc);
    UNUSED(argv);

    /* rterm checks every SDL2 call, so: */
    try {
        /* create empty terminal with 80 columns and 24 rows */
        rterm::Term terminal(80, 24);
        terminal.setResizable(false);
        /* set window title */
        terminal.setTitle("rterm example");
        /* load TrueType font */
        int fontSize = 18 * 96 / 72;
        terminal.setFont("resources/DejaVuSansMono.ttf", fontSize);
        terminal.setIcon("resources/terminal.ico");
        /* set foreground color to green (#00FF00) */
        terminal.setFgColor(rterm::Color(0x00, 0xff, 0x00));
        /* set callback for keypress */
        bool wasInput = false;
        terminal.onKeyDown([&](const rterm::events::KeyDownEvent& ev) {
            /* if key has visual representation... */
            if (ev.key().toChar()) {
                /* add character to terminal */
                terminal.addChar(ev.key().toChar());
                /* and draw everything on screen */
                terminal.redraw();

                auto start = std::chrono::high_resolution_clock::now();
                terminal.redraw();
                auto end = std::chrono::high_resolution_clock::now();
                double secs = std::chrono::duration_cast<std::chrono::duration<double, std::micro>>(end - start).count();
                rterm::Logger() << "Keypress: " << secs << "µs";
            }
        });
        /* while terminal is not closed... */
        while (terminal.isRunning()) {
            /* poll events */
            // auto start = std::chrono::high_resolution_clock::now();
            terminal.poll();
            // auto end = std::chrono::high_resolution_clock::now();
            if (wasInput) {
                // double secs = std::chrono::duration_cast<std::chrono::duration<double, std::micro>>(end - start).count();
                // rterm::Logger() << "Keypress: " << secs << "µs";
            }
            wasInput = false;
        }
        terminal.dumpScreen().save("/home/sergey/term.bmp");
        return 0;
    } catch (const rterm::Exception& e) {
        rterm::Logger() << e.what();
        return 0;
    }
}
