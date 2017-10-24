#include "term.h"
#include "font.h"
#include <iostream>

namespace term {
Term::Term()
    : Term(0, 0) {
}

Term::Term(size_t cols, size_t rows)
    : _cols(cols),
      _rows(rows),
      data(_rows * _cols, ' '),
      mask(_rows * _cols, false),
      cursorPos(),
      quitRequested(false) {
    SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    SDL_CreateWindowAndRenderer(_cols * font.w(), _rows * font.h(), 0, &p_win, &p_ren);
    SDL_RenderFillRect(p_ren, NULL);
    SDL_SetEventFilter(eventFilter, this);
    SDL_RenderPresent(p_ren);
    font = Font(p_ren);
}

Term::~Term() {
    SDL_DestroyRenderer(p_ren);
    SDL_DestroyWindow(p_win);
    SDL_Quit();
}

size_t Term::cols() const {
    return _cols;
}

size_t Term::rows() const {
    return _rows;
}

bool Term::running() const {
    return !quitRequested;
}

char& Term::get(size_t x, size_t y) {
    return data[y * _cols + x];
}

char Term::get(size_t x, size_t y) const {
    return data[y * _cols + x];
}

void Term::resize(size_t _rows, size_t _cols) {
    (void)_rows;
    (void)_cols;
}

void Term::setChar(size_t x, size_t y, char c) {
    mask[y * _cols + x] = true;
    get(x, y) = c;
}

void Term::addChar(char c) {
    switch (c) {
    case '\n':
    case '\r':
        cursorPos = (cursorPos + _cols - cursorPos % _cols) % data.size();
        break;
    case '\b':
        cursorPos = cursorPos ? cursorPos - 1: 0;
        data[cursorPos] = ' ';
        mask[cursorPos] = true;
        break;
    case '\t':
        for (int i = 0; i < 4; ++i)
            addChar(' ');
        break;
    default:
        mask[cursorPos] = true;
        data[cursorPos++] = c;
        cursorPos %= data.size();
    }
}

SDL_Keycode Term::getKey() const {
    SDL_Event ev = getNextEvent({SDL_KEYDOWN});
    if (running())
        return ev.key.keysym.sym;
}

char Term::getChar() const {
    SDL_StartTextInput();
    while (true) {
        SDL_Event ev = getNextEvent({SDL_TEXTINPUT, SDL_KEYDOWN});
        if (!running())
            return 0;
        switch (ev.type) {
        case SDL_TEXTINPUT:
            SDL_StopTextInput();
            return ev.text.text[0];
        case SDL_KEYDOWN:
            if (ev.key.keysym.sym == SDLK_RETURN ||
                ev.key.keysym.sym == SDLK_BACKSPACE ||
                    ev.key.keysym.sym == SDLK_TAB) {
                SDL_StopTextInput();
                return ev.key.keysym.sym;
            }
            break;
        default:
            break;
        }
    }
}

char Term::charAt(size_t x, size_t y) const {
    return get(x, y);
}

void Term::setFullscreen() {
}

void Term::setFont(const Font &f) {
    /** TODO: copy constructor */
    //font = f;
    SDL_SetWindowSize(p_win, _cols * font.w(), _rows * font.h());
    redraw();
}

void Term::redraw() {
    bool changed = false;
    for (size_t x = 0; x < _cols; ++x)
        for (size_t y = 0; y < _rows; ++y) {
            if (mask[y * _cols + x])
                redraw(x, y), changed = true;
            mask[y * _cols + x] = false;
        }
    if (changed)
        SDL_RenderPresent(p_ren);
}

void Term::redraw(size_t x, size_t y) {
    SDL_Rect dst {x * font.w(), y * font.h(), font.w(), font.h()};
    SDL_Rect src = font.getRect(get(x, y));
    SDL_RenderFillRect(p_ren, &dst);
    SDL_RenderCopy(p_ren, font.getTexture(), &src, &dst);
}

SDL_Event Term::getNextEvent(const std::set<int> &filter) const {
    SDL_Event ev;
    while (true) {
        if (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT || (filter.find(ev.type) != filter.end()))
                return ev;
        }
        SDL_Delay(EVENT_POLL_DELAY);
    }
}

int eventFilter(void *data, SDL_Event *ev) {
    Term *term = reinterpret_cast<Term*>(data);
    switch (ev->type) {
    case SDL_QUIT:
        term->quitRequested = true;
        break;
    default:
        break;
    }
}
}
