#include <rterm/rterm.h>

#include <chrono>
#include <cstring>
#include <iostream>
#include <random>

int main(int, char**) {
    rterm::Logger() << "Benchmark";

    try {
        rterm::Term terminal(80, 24);
        terminal.setMaxWindowSize(2000, 2000);
        terminal.setMinWindowSize(200, 100);
        int fontSize = 18;
        terminal.setTitle("rterm demo");
        terminal.setFont("resources/DejaVuSansMono.ttf", fontSize);
        terminal.setIcon("resources/icon.png");
        terminal.setFgColor(rterm::Color(100, 0xff, 100));
        terminal.setFullscreen(false);
        terminal.setResizable(true);
        int flooditers = 5000;
        int randomiters = 40000;
        int frames = 0;
        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
        for (int iters = 0; iters < flooditers; ++iters) {
            for (int i = 0; i < int(terminal.cols()); ++i)
                for (int j = 0; j < int(terminal.rows()); ++j) {
                    terminal.setChar(i, j, 'a' + (i + j + iters) % ('z' - 'a'));
                    terminal.setBgColor(
                        rterm::Color(
                            std::min<unsigned>(i * 180 / terminal.cols() + iters * 127 / 1000, 0xff),
                            0,
                            std::min<unsigned>(j * 180 / terminal.cols() + iters * 127 / 1000, 0xff)
                        ), i, j
                    );
                    terminal.setFgColor(
                        rterm::Color(
                            0xff - std::min<unsigned>(i * 180 / terminal.cols() + iters * 127 / 1000, 0xff),
                            0xff,
                            0xff - std::min<unsigned>(j * 180 / terminal.cols() + iters * 127 / 1000, 0xff)
                        ), i, j
                    );
            }
            terminal.print(0, terminal.rows() - 1, "FPS = %d ", int(terminal.fps()));
            terminal.poll();
            terminal.redraw();
            ++frames;
            if (iters % 100 == 0) {
                rterm::Logger() << "Full redraw : " << iters << " / " << flooditers;
            }
            if (!terminal.isRunning())
                return 0;
        }

        std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::ratio<1, 1>> deltaTime = end - start;
        rterm::Logger().printf("Flood time usage: %f s", deltaTime.count());
        rterm::Logger().printf("FPS: %f", frames / deltaTime.count());
        start = end;
        frames = 0;

        for (int iters = 0; iters < randomiters; ++iters) {
            int i = rand() % terminal.cols();
            int j = rand() % terminal.rows();
            terminal.setChar(i, j, 'a' + rand() % ('z' - 'a'));
            rterm::Color bg(
                std::min<int>(i * 0xff / terminal.cols(), 0xff),
                0,
                std::min<int>(j * 0xff / terminal.cols(), 0xff)
            );
            rterm::Color fg(
                0xff - std::min<int>(i * 0xff / terminal.cols(), 0xff),
                0xff,
                0xff - std::min<int>(j * 0xff / terminal.cols(), 0xff)
            );
            terminal.setBgColor(bg, i, j);
            terminal.setFgColor(fg, i, j);
            terminal.print(0, terminal.rows() - 1, "FPS = %d ", int(terminal.fps()));
            terminal.poll();
            terminal.redraw();
            ++frames;
            if (iters % 100 == 0) {
                rterm::Logger() << "Random : " << iters << " / " << randomiters;
            }
        }
        end = std::chrono::high_resolution_clock::now();
        deltaTime = end - start;
        rterm::Logger().printf("Random time usage: %f s", deltaTime.count());
        rterm::Logger().printf("FPS: %f", frames / deltaTime.count());
        return 0;
    } catch (const rterm::Exception& e) {
        rterm::Logger() << e.what();
        return 1;
    }
}
