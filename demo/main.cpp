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
    terminal.setFont("../fonts/ttf/DejaVuSansMono.ttf", ++fontSize);
    terminal.setFgColor(rterm::Color(100, 255, 100));
    terminal.setFullscreen(false);
    terminal.setResizable(true);
    rterm::Key k = terminal.getKey();
    int flooditers = 0, randomiters = 2000;
    if (argc > 1 && !strcmp(argv[1], "--benchmark"))
        flooditers = 2000, randomiters = 10000;
    else if (argc > 1 && !strcmp(argv[1], "--test"))
        flooditers = 100, randomiters = 100;
    for (int iters = 0; iters < randomiters; ++iters) {
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
    for (int i = 0; i < randomiters; ++i) {
        terminal.shift((rand() % 3) - 1, (rand() % 3) - 1);
        terminal.print(0, terminal.rows() - 1, "FPS = %d ", int(terminal.fps()));
        terminal.redraw();
    }
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    for (int iters = 0; iters < flooditers; ++iters) {
        terminal.shift(1, 0);
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
        terminal.print(0, terminal.rows() - 1, "FPS = %d ", int(terminal.fps()));
        terminal.redraw();
    }
    
    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::ratio<1, 1>> deltaTime = end - start;
    SDL_Log("Flood time usage: %f s", deltaTime.count());
    start = end;

    for (int iters = 0; iters < randomiters; ++iters) {
        int i = rand() % terminal.cols(),
            j = rand() % terminal.rows();
        terminal.setChar(i, j, 'a' + rand() % ('z' - 'a'));
        terminal.setBgColor(rterm::Color(std::min(i * 255 / terminal.cols(), (size_t)255ull),
                                         0,
                                         std::min(j * 255 / terminal.cols(), (size_t)255ull)), i, j);
        terminal.setFgColor(rterm::Color(255 - std::min(i * 255 / terminal.cols(), (size_t)255ull),
                                         255,
                                         255 - std::min(j * 255 / terminal.cols(), (size_t)255ull)), i, j);
        terminal.print(0, terminal.rows() - 1, "FPS = %d ", int(terminal.fps()));
        terminal.redraw();
    }
    end = std::chrono::high_resolution_clock::now();
    deltaTime = end - start;
    SDL_Log("Random time usage: %f s", deltaTime.count());

    terminal.setFgColor(rterm::Color(255,  0,  0));
    terminal.setBgColor(rterm::Color(0, 255, 255));

    terminal.delay(1000);
    if (argc > 1 && !strcmp(argv[1], "--test")) {
        for (int i = 0; i < 1000; ++i)
            terminal.setChar(rand() % terminal.cols(), rand() % terminal.rows(), 'a' + rand() % 26);
    } else {
        bool fullscr = false;
        while(terminal.isRunning()) {
            rterm::Key k = terminal.getKey();
            if (k.key() == rterm::F4 && k.mod() & rterm::ALT)
                return 0;
            SDL_Log("%s", SDL_GetKeyName(k.key()));
            switch (k.key()) {
            case rterm::F1:
                terminal.setFullscreen(!fullscr);
                fullscr = !fullscr;
                break;
            case rterm::UP:
                terminal.shift(0, -1);
                break;
            case rterm::LEFT:
                terminal.shift(-1, 0);
                break;
            case rterm::RIGHT:
                terminal.shift(1, 0);
                break;
            case rterm::DOWN:
                terminal.shift(0, 1);
                break;
            default:
                break;
            }
            // if (k.toChar())
            //    terminal.addChar(k.toChar());
            terminal.redraw();
        }
    }
    return 0;
}
