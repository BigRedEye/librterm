#ifndef TERM_H
#define TERM_H

#include <cstddef>
#include <vector>
#include <SDL2/SDL.h>

namespace term {

class Event;
class Font;

class Term {
public:
    Term();
    Term(size_t rows, size_t cols);
    ~Term();

    void resize(size_t _rows, size_t _cols);
    void setChar(size_t x, size_t y, char c);
    void setFont(const Font &f);
    void setFullscreen();

    void redraw();
    void redraw(size_t x, size_t y);

    char getChar(size_t x, size_t y);
    Event getNextEvent();

private:
    char& get(size_t x, size_t y);

    size_t rows;
    size_t cols;
    Font font;

    std::vector<char> data;
    std::vector<char> mask;

    SDL_Window *p_win;
    SDL_Renderer *p_ren;
};

}
#endif // TERM_H
