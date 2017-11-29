#include "../include/term.h"
#include "../include/key.h"
#include <iostream>
#include <random>
#include <complex>
#include <cstring>
#include <chrono>

#define UNUSED(cond) (void)(cond)

int main(int argc, char **argv)
{
    UNUSED(argc);
    UNUSED(argv);

    rterm::Term terminal(80, 24);
    terminal.setMaxWindowSize(2000, 2000);
    terminal.setMinWindowSize(200, 100);
    int fontSize = 18;
    terminal.setTitle("rterm demo")
            .setIcon("terminal.ico");
    //terminal.setFont("../fonts/tile/10x10.jpg", 10, 10);
    terminal.setFont("../fonts/ttf/DejaVuSansMono.ttf", ++fontSize);
    terminal.setFgColor(rterm::Color(100, 255, 100));
    terminal.setFullscreen(false);
    terminal.setResizable(true);
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    for (int iters = 0; iters < 1000; ++iters) {
        for (int i = 0; i < terminal.cols(); ++i)
            for (int j = 0; j < terminal.rows(); ++j) {
                terminal.setChar(i, j, 'a' + (i + j + iters) % ('z' - 'a'));
                terminal.setBgColor(rterm::Color(std::min(i * 180 / terminal.cols() + iters * 127 / 1000, (size_t)255ull),
                                                 0,
                                                 std::min(j * 180 / terminal.cols() + iters * 127 / 1000, (size_t)255ull)), i, j);
                terminal.setFgColor(rterm::Color(255 - std::min(i * 180 / terminal.cols() + iters * 127 / 1000, (size_t)255ull),
                                                 255,
                                                 255 - std::min(j * 180 / terminal.cols() + iters * 127 / 1000, (size_t)255ull)), i, j);
        }
        terminal.redraw();
    }
    for (int iters = 0; iters < 100000; ++iters) {
        int i = rand() % terminal.cols(),
            j = rand() % terminal.rows();
        terminal.setChar(i, j, 'a' + rand() % ('z' - 'a'));
        terminal.setBgColor(rterm::Color(std::min(i * 255 / terminal.cols(), (size_t)255ull),
                                         0,
                                         std::min(j * 255 / terminal.cols(), (size_t)255ull)), i, j);
        terminal.setFgColor(rterm::Color(255 - std::min(i * 255 / terminal.cols(), (size_t)255ull),
                                         255,
                                         255 - std::min(j * 255 / terminal.cols(), (size_t)255ull)), i, j);
        terminal.redraw();
    }
    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::ratio<1, 1>> deltaTime = end - start;
    SDL_Log("deltaTime: %f mscec", deltaTime.count());
    terminal.setFgColor(rterm::Color(255,  0,  0));
    terminal.setBgColor(rterm::Color(0, 255, 255));
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
