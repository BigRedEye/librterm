#include "term.h"
#include <iostream>
#include <random>
#include "key.h"

#define UNUSED(cond) (void)(cond)

int main(int argc, char **argv)
{
    UNUSED(argc);
    UNUSED(argv);

    term::Term terminal(40, 20);
    int x = 10, y = 5;
    int i = 0;
    for (int i = 0; i < terminal.cols(); ++i)
        for (int j = 0; j < terminal.rows(); ++j) {
            terminal.setChar(i, j, ' ');
            terminal.setBgColor(term::Color((i + j) % 255, 0, (i + j) % 255), i, j);
            terminal.setFgColor(term::Color(255 - (i + j) % 255, 0, 255 - (i + j) % 255), i, j);
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
