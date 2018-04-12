#include <rterm/term.h>
#include <rterm/key.h>
#include <rterm/mouse.h>

#include <iostream>
#include <random>
#include <cstring>
#include <chrono>

#define UNUSED(cond) (void)(cond)

int main(int argc, char **argv)
{
    UNUSED(argc);
    UNUSED(argv);

    rterm::Term terminal(80, 24);
    int fontSize = 18;
    terminal.setTitle("rterm demo")
            .setIcon("terminal.ico");
    terminal.setFont("../fonts/ttf/DejaVuSansMono.ttf", fontSize);
    terminal.setResizable(true);
    terminal.resize(80, 24);

    terminal.setFgColor(rterm::Color(0, 0xff, 0));

    bool fullscr = false;
    while (terminal.isRunning()) {
        rterm::Key k = terminal.getKey(16);
        if (k.key() == rterm::KeyCode::F4 && (k.mod() & rterm::KeyMod::ALT))
            return 0;
        switch (k.key()) {
        case rterm::KeyCode::F1:
            fullscr = !fullscr;
            terminal.setFullscreen(fullscr);
            break;
        case rterm::KeyCode::UP:
            terminal.shift(0, -1);
            break;
        case rterm::KeyCode::LEFT:
            terminal.shift(-1, 0);
            break;
        case rterm::KeyCode::RIGHT:
            terminal.shift(1, 0);
            break;
        case rterm::KeyCode::DOWN:
            terminal.shift(0, 1);
            break;
        default:
            break;
        }
        static int mx = -1,
                   my = -1;
        size_t x = 0,
               y = 0;
        terminal.getMousePosition(x, y);
        if (terminal.getMouseButtons() & rterm::MouseButton::LEFT)
            terminal.setBgColor(rterm::Color(0xff, 0xff, 0xff), x, y);
        if (terminal.getMouseButtons() & rterm::MouseButton::RIGHT) {
            if (mx >= 0 && my >= 0)
                terminal.shift(x - mx, y - my);
            mx = x;
            my = y;
        } else {
            mx = -1;
            my = -1;
        }

        if (k.toChar())
            terminal.addChar(k.toChar());
        terminal.redraw();
    }
    return 0;
}
