#include <rterm/error.h>
#include <rterm/logger.h>
#include <rterm/term.h>

#include <chrono>
#include <cstring>
#include <iostream>
#include <random>

#define UNUSED(var) (void)var

int main(int argc, char** argv) {
    rterm::Logger() << "Benchmark";
    UNUSED(argc);
    UNUSED(argv);

    try {
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
        int flooditers = 500;
        int randomiters = 4000;
        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
        for (int iters = 0; iters < flooditers; ++iters) {
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
            terminal.poll();
            terminal.redraw();
            if (iters % 100 == 0) {
                rterm::Logger() << "Full redraw : " << iters << " / " << flooditers;
            }
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
            terminal.poll();
            terminal.redraw();
            if (iters % 100 == 0) {
                rterm::Logger() << "Random : " << iters << " / " << randomiters;
            }
        }
        end = std::chrono::high_resolution_clock::now();
        deltaTime = end - start;
        rterm::Logger().printf("Random time usage: %f s", deltaTime.count());
    } catch (const rterm::Exception& e) {
        rterm::Logger(rterm::Logger::CRITICAL) << "Cought an exception: " << e.what();
    }
    return 0;
}
