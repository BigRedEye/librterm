#ifndef TERM_H
#define TERM_H

#include <cstddef>
#include <vector>
#include <set>
#include <SDL2/SDL.h>

#include "font.h"
#include "event.h"
#include "key.h"

namespace term {

class Term {
public:
    Term();
    Term(size_t cols, size_t rows);
    ~Term();

    bool running() const;

    void resize(size_t _cols, size_t _rows);
    void setChar(size_t x, size_t y, char c);
    void setFont(const Font &f);
    void setFullscreen();
    size_t cols() const;
    size_t rows() const;

    SDL_Keycode getKey() const;
    char getChar() const;
    void addChar(char c);

    void redraw();
    void redraw(size_t x, size_t y);

    char charAt(size_t x, size_t y) const;
    SDL_Event getNextEvent(const std::set<int> &filter) const;

    friend int eventFilter(void *data, SDL_Event *ev);
private:
    char& get(size_t x, size_t y);
    char get(size_t x, size_t y) const;

    static constexpr int EVENT_POLL_DELAY = 10;

    size_t _cols;
    size_t _rows;
    Font font;

    std::vector<char> data;
    std::vector<char> mask;

    SDL_Window *p_win;
    SDL_Renderer *p_ren;

    size_t cursorPos;
    bool quitRequested;
};

int eventFilter(void *data, SDL_Event *ev);
}
#endif // TERM_H
