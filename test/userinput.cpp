#include <rterm/rterm.h>

#include <chrono>

/*
 * on Windows SDL2 redefines main with custom SDL_main function,
 * so we need argc and argv
 */
int main(int, char**) {
    try {
        /* create empty terminal with 80 columns and 24 rows */
        rterm::Term terminal(80, 24);
        terminal.setResizable(false);
        /* set window title */
        terminal.setTitle("rterm example");
        /* load TrueType font */
        int fontSize = 18 * 96 / 72;
        terminal.setFont("resources/DejaVuSansMono.ttf", fontSize);
        terminal.setIcon("resources/icon.png");
        /* set foreground color to green (#00FF00) */
        terminal.setFgColor(rterm::Color::Green);
        /* set callback for keypress */
        bool wasInput = false;
        terminal.onKeyDown([&](const rterm::events::KeyDownEvent& ev) {
            /* if key has visual representation... */
            if (ev.key().toChar()) {
                /* add character to terminal */
                terminal.addChar(ev.key().toChar());
                /* and draw everything on screen */
                terminal.redraw();
                wasInput = true;
            }
        });
        /* while terminal is not closed... */
        while (terminal.isRunning()) {
            /* measure polling time */
            auto start = std::chrono::steady_clock::now();
            /* poll events */
            terminal.poll();
            /* if was keypress: */
            auto end = std::chrono::steady_clock::now();
            if (wasInput) {
                double secs = std::chrono::duration_cast<std::chrono::duration<double, std::micro>>(end - start).count();
                rterm::Logger() << "Keypress: " << secs << "µs";
            }
            wasInput = false;
        }
        return 0;
    } catch (const rterm::Exception& e) {
        rterm::Logger() << e.what();
        return 1;
    }
}
