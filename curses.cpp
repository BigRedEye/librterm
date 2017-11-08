#include <ncurses.h>
#include <iostream>

int main() {
    initscr();
    int i = getch();
    endwin();
    std::cout << i << std::endl;
    return 0;
}
