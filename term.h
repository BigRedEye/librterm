#ifndef TERM_H
#define TERM_H

#include <cstddef>
#include <vector>
#include <set>
#include <SDL2/SDL.h>

#include "font.h"
#include "event.h"
#include "key.h"
#include "color.h"

namespace term {

class Term {
public:
    Term();
    Term(size_t cols, size_t rows);
    ~Term();

    bool running() const;

    void resize(size_t cols_, size_t rows_);
    void setChar(size_t x, size_t y, char c);
    void setFont(const Font &f);
    void setFullscreen();
    size_t cols() const;
    size_t rows() const;

    Key getKey() const;
    char getChar() const;
    void addChar(char c);

    void redraw();
    void redraw(size_t x, size_t y);

    char charAt(size_t x, size_t y) const;
    SDL_Event getNextEvent(const std::set<int> &filter) const;

    friend int eventFilter(void *data_, SDL_Event *ev);
private:
    char& get(size_t x, size_t y);
    char get(size_t x, size_t y) const;

    static constexpr int EVENT_POLL_DELAY = 10;

    size_t cols_;
    size_t rows_;
    Font font_;

    std::vector<char> data_;
    std::vector<char> mask_;

    SDL_Window *p_win_;
    SDL_Renderer *p_ren_;

    size_t cursorPos_;
    bool quitRequested_;

    Color fgCol_;
    Color bgCol_;
};

int eventFilter(void *data, SDL_Event *ev);
}
#endif // TERM_H
