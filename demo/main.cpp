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

    rterm::Term terminal;
    terminal.setMaxWindowSize(2000, 2000);
    terminal.setMinWindowSize(200, 100);
    SDL_Log("%d", terminal.cols());
    SDL_Log("%d", terminal.rows());
    int fontSize = 10;
    terminal.setFont("../fonts/tile/10x10.jpg", 10, 10);
    terminal.setFgColor(rterm::Color(100, 255, 100));
    terminal.setFullscreen(false);
    terminal.setResizable(true);

    if (argc > 1 && !strcmp(argv[1], "--test")) {
        for (int i = 0; i < 1000; ++i)
            terminal.setChar(rand() % terminal.cols(), rand() % terminal.rows(), 'a' + rand() % 26);
    } else {
        bool fullscr = false;
        while(terminal.isRunning()) {
            rterm::Key k = terminal.getKey();
            if (k.key() == rterm::F4 && k.mod() & rterm::ALT)
                return 0;
            if (k.key() == rterm::F1) {
                terminal.setFullscreen(!fullscr);
                fullscr = !fullscr;
            }
            if ((k.key() == rterm::NP_PLUS && k.mod() & rterm::CTRL) || (k.key() == '=' && k.mod() & (rterm::CTRL | rterm::SHIFT)))
                terminal.setFont("../fonts/ttf/DejaVuSansMono.ttf", ++fontSize);
            else if (k.toChar())
                terminal.addChar(k.toChar());
            terminal.redraw();
        }
    }
    return 0;
}
