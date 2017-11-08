#include "term.h"
#include <iostream>
#include <random>
#include <complex>
#include "key.h"

#define UNUSED(cond) (void)(cond)

int main(int argc, char **argv)
{
    UNUSED(argc);
    UNUSED(argv);

    term::Term terminal(200, 100);
    int iter = 32;
    for (int i = 0; i < terminal.cols(); ++i)
        for (int j = 0; j < terminal.rows(); ++j) {
            terminal.setChar(i, j, ' ');
            std::complex<long double> c(i * 2. / terminal.cols() - 1.5,
                                        j * 2. / terminal.rows() - 1),
                                      z(0, 0);
            int k = 0;
            for (k; k < iter; ++k) {
                z = z * z + c;
                if (abs(z) > 2)
                    break;
            }
            terminal.setBgColor(term::Color(k * 255 / iter, 0.7 * k * 255 / iter, 0.1 * k * 255 / iter), i, j);
            terminal.setFgColor(term::Color(255 - k * 255 / iter, 255 - k * 255 / iter, 255 - k * 255 / iter), i, j);
        }
    terminal.redraw();

    while (terminal.running()) {
        terminal.addChar(terminal.getChar());
        terminal.delay(10);
        /*
        terminal.setBgColor(term::Color(i % 256, i % 256, 0),
                            i / terminal.rows(),
                            i % terminal.rows());
        terminal.setFgColor(term::Color(255 - i % 256, 255 - i % 256, 0),
                            i / terminal.rows(),
                            i % terminal.rows());*/

        /*term::Key key = terminal.getKey();
        switch (key.key()) {
        case term::UP:
            terminal.setChar(x % terminal.cols(), y % terminal.rows(), ' ');
            --y;
            terminal.setChar(x % terminal.cols(), y % terminal.rows(), '@');
            break;
        case term::DOWN:
            terminal.setChar(x % terminal.cols(), y % terminal.rows(), ' ');
            ++y;
            terminal.setChar(x % terminal.cols(), y % terminal.rows(), '@');
            break;
        case term::LEFT:
            terminal.setChar(x % terminal.cols(), y % terminal.rows(), ' ');
            --x;
            terminal.setChar(x % terminal.cols(), y % terminal.rows(), '@');
            break;
        case term::RIGHT:
            terminal.setChar(x % terminal.cols(), y % terminal.rows(), ' ');
            ++x;
            terminal.setChar(x % terminal.cols(), y % terminal.rows(), '@');
            break;
        default:
            break;
        }*/
        terminal.redraw();
    }
    return 0;
}
