#include <rterm/rterm.h>

/*
 * on Windows SDL2 redefines main with custom SDL_main function,
 * so we need argc and argv
 */
int main(int, char**) {
    try {
        /* create empty terminal with 80 columns and 24 rows */
        rterm::TermFormat format(80, 24);
        rterm::Term terminal(format);
        /* set window title */
        terminal.setTitle("mouse");
        /* load TrueType font */
        rterm::ui32 fontSize = 18;
        terminal.setFont("resources/DejaVuSansMono.ttf", fontSize);
        terminal.setIcon("resources/icon.png");
        /* set foreground color to green (#00FF00) */
        terminal.setFgColor(rterm::Color::Blue);
        /* set callback for keypress */
        int x = 0;
        int y = 0;
        terminal.onMouseMove([&](const rterm::events::MouseMoveEvent& ev) {
            if (ev.x() != x || ev.y() != y) {
                auto setCh = [&](char32_t ch, int xx, int yy) {
                    if (x >= 0 && y >= 0) {
                        terminal.setChar(x / 32, y / 32, ch);
                        x = xx;
                        y = yy;
                    }
                };
                setCh(' ', ev.x(), ev.y());
                setCh('@', ev.x(), ev.y());
                terminal.redraw();
            }
        });
        /* while terminal is not closed... */
        while (terminal.isRunning()) {
            /* poll events
             * note that registered callbacks can be
             * executed during this function call only
             */
            terminal.poll();
        }
    } catch (const rterm::Exception& e) {
        rterm::Logger() << e.what();
    }
    return 0;
}
