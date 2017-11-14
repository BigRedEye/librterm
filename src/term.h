#ifndef TERM_H
#define TERM_H

#include <cstddef>
#include <vector>
#include <set>
#include <SDL2/SDL.h>

#include "font.h"
#include "key.h"
#include "color.h"
#include "char.h"

namespace term {
typedef char32_t char_t;

class Term {
public:
    Term();
    Term(size_t cols, size_t rows);
    ~Term();

    bool running() const;
    size_t cols() const;
    size_t rows() const;
    void delay(uint32_t msec) const;

    void resize(size_t cols_, size_t rows_);
    void setChar(size_t x, size_t y, char_t c);
    void setFont(const std::string &s, size_t sz);
    void setBgColor(const Color &bg);
    void setBgColor(const Color &bg, size_t x, size_t y);
    void setFgColor(const Color &fg);
    void setFgColor(const Color &fg, size_t x, size_t y);

    void setFullscreen();

    /* wait for next key */
    Key getKey() const;
    /* wait for next ASCII char */
    char_t getChar() const;

    void addChar(char_t c);

    void redraw();
    void redraw(size_t x, size_t y);

    char_t charAt(size_t x, size_t y) const;
    SDL_Event getNextEvent(const std::set<int> &filter) const;

    friend int eventFilter(void *data_, SDL_Event *ev);
private:
    class Char;

    Char& get(size_t x, size_t y);
    Char get(size_t x, size_t y) const;

    static constexpr int EVENT_POLL_DELAY = 10;

    size_t cols_;
    size_t rows_;
    Font font_;

    std::vector<Char> data_;
    std::vector<char> mask_;

    SDL_Window *p_win_;
    SDL_Renderer *p_ren_;

    size_t cursorPos_;
    bool quitRequested_;

    Color fgCol_;
    Color bgCol_;
};

class Term::Char {
public:
    Char(char_t ch = 0, Color bg = Color(0, 0, 0), Color fg = Color(255, 255, 255))
        : ch_(ch), bg_(bg), fg_(fg) {}

    friend class Term;
private:
    char_t ch_;
    Color bg_, fg_;
};

int eventFilter(void *data, SDL_Event *ev);
}
#endif // TERM_H
