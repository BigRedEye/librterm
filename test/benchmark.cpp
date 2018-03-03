#include <rterm/term.h>
#include <rterm/logger.h>

#include <iostream>
#include <random>
#include <cstring>
#include <chrono>

#define UNUSED(cond) (void)(cond)

int main(int argc, char **argv) {
    UNUSED(argc);
    UNUSED(argv);

    rterm::Term terminal(80, 24);
    terminal.setMaxWindowSize(2000, 2000);
    terminal.setMinWindowSize(200, 100);
    int fontSize = 18;
    terminal.setTitle("rterm demo")
            .setIcon("terminal.ico");
    terminal.setFont("../fonts/ttf/DejaVuSansMono.ttf", fontSize);
    terminal.setFgColor(rterm::Color(100, 0xff, 100));
    terminal.setFullscreen(false);
    terminal.setResizable(true);
    terminal.resize(80, 24);
    int flooditers = 500, randomiters = 4000;
    for (int iters = 0; iters < randomiters; ++iters) {
        int i = rand() % terminal.cols(),
            j = rand() % terminal.rows();
        terminal.setChar(i, j, 'a' + rand() % ('z' - 'a'));
        terminal.setBgColor(rterm::Color(std::min(i * 0xff / terminal.cols(), (size_t)0xffull),
                                         0,
                                         std::min(j * 0xff / terminal.cols(), (size_t)0xffull)), i, j);
        terminal.setFgColor(rterm::Color(0xff - std::min(i * 0xff / terminal.cols(), (size_t)0xffull),
                                         0xff,
                                         0xff - std::min(j * 0xff / terminal.cols(), (size_t)0xffull)), i, j);
        terminal.redraw();
        if (!terminal.isRunning())
            return 0;
    }
    for (int i = 0; i < randomiters; ++i) {
        terminal.print(0, terminal.rows() - 1, "FPS = %d ", int(terminal.fps()));
        terminal.redraw();
        if (!terminal.isRunning())
            return 0;
    }
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    for (int iters = 0; iters < flooditers; ++iters) {
        terminal.shift(1, 0);
        for (int i = 0; i < int(terminal.cols()); ++i)
            for (int j = 0; j < int(terminal.rows()); ++j) {
                terminal.setChar(i, j, 'a' + (i + j + iters) % ('z' - 'a'));
                terminal.setBgColor(rterm::Color(std::min(i * 180 / terminal.cols() + iters * 127 / 1000, (size_t)0xffull),
                                                 0,
                                                 std::min(j * 180 / terminal.cols() + iters * 127 / 1000, (size_t)0xffull)), i, j);
                terminal.setFgColor(rterm::Color(0xff - std::min(i * 180 / terminal.cols() + iters * 127 / 1000, (size_t)0xffull),
                                                 0xff,
                                                 0xff - std::min(j * 180 / terminal.cols() + iters * 127 / 1000, (size_t)0xffull)), i, j);
        }
        terminal.print(0, terminal.rows() - 1, "FPS = %d ", int(terminal.fps()));
        terminal.redraw();
        if (!terminal.isRunning())
            return 0;
    }

    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::ratio<1, 1>> deltaTime = end - start;
    rterm::Logger().printf("Flood time usage: %f s", deltaTime.count());
    start = end;

    for (int iters = 0; iters < randomiters; ++iters) {
        int i = rand() % terminal.cols(),
            j = rand() % terminal.rows();
        terminal.setChar(i, j, 'a' + rand() % ('z' - 'a'));
        rterm::Color bg(std::min(i * 0xff / terminal.cols(), (size_t)0xffull),
                        0,
                        std::min(j * 0xff / terminal.cols(), (size_t)0xffull));
        rterm::Color fg(0xff - std::min(i * 0xff / terminal.cols(), (size_t)0xffull),
                        0xff,
                        0xff - std::min(j * 0xff / terminal.cols(), (size_t)0xffull));
        terminal.setBgColor(bg, i, j);
        terminal.setFgColor(fg, i, j);
        terminal.print(0, terminal.rows() - 1, "FPS = %d ", int(terminal.fps()));
        terminal.redraw();
    }
    end = std::chrono::high_resolution_clock::now();
    deltaTime = end - start;
    rterm::Logger().printf("Random time usage: %f s", deltaTime.count());

    terminal.setFgColor(rterm::Color(0, 0xff, 0));
    terminal.setBgColor(rterm::Color(0, 0, 0));

    terminal.delay(1000);
    for (int i = 0; i < 1000; ++i)
        terminal.setChar(rand() % terminal.cols(),
                         rand() % terminal.rows(),
                         'a' + rand() % ('z' - 'a'));
    std::cout << "Done" << std::endl;
    return 0;
}
