#include "term.h"
#include "font.h"
#include <iostream>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

namespace term {
Term::Term()
    : Term(0, 0) {
}

Term::Term(size_t cols, size_t rows)
    : cols_(cols),
      rows_(rows),
      data_(rows * cols, '.'),
      mask_(rows * cols, true),
      cursorPos_(),
      quitRequested_(false),
      fgCol_(255, 255, 255),
      bgCol_(255, 0, 0),
      font_() {
    SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();


    p_win_ = SDL_CreateWindow("Terminal", SDL_WINDOWPOS_UNDEFINED,
                                         SDL_WINDOWPOS_UNDEFINED,
                                         cols_ * 10, rows_ * 10, 0);
    font_ = Font(); /* we need to call TTF_Init() before */
    SDL_SetWindowSize(p_win_, font_.w() * cols_, font_.h() * rows_);
    p_ren_ = SDL_CreateRenderer(p_win_, 0, 0);
    font_.setRenderer(p_ren_);
    SDL_SetEventFilter(eventFilter, this);
    SDL_RenderFillRect(p_ren_, NULL);
    redraw();
}

Term::~Term() {
    SDL_DestroyRenderer(p_ren_);
    SDL_DestroyWindow(p_win_);

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

size_t Term::cols() const {
    return cols_;
}

size_t Term::rows() const {
    return rows_;
}

bool Term::running() const {
    return !quitRequested_;
}

char& Term::get(size_t x, size_t y) {
    return data_[y * cols_ + x];
}

char Term::get(size_t x, size_t y) const {
    return data_[y * cols_ + x];
}

void Term::resize(size_t _rows, size_t _cols) {
    (void)_rows;
    (void)_cols;
}

void Term::setChar(size_t x, size_t y, char c) {
    mask_[y * cols_ + x] = true;
    get(x, y) = c;
}

void Term::addChar(char c) {
    switch (c) {
    case '\n':
    case '\r':
        cursorPos_ = (cursorPos_ + cols_ - cursorPos_ % cols_) % data_.size();
        break;
    case '\b':
        cursorPos_ = cursorPos_ ? cursorPos_ - 1: 0;
        data_[cursorPos_] = ' ';
        mask_[cursorPos_] = true;
        break;
    case '\t':
        for (int i = 0; i < 4; ++i)
            addChar(' ');
        break;
    default:
        mask_[cursorPos_] = true;
        data_[cursorPos_++] = c;
        cursorPos_ %= data_.size();
    }
}

Key Term::getKey() const {
    SDL_Event ev = getNextEvent({SDL_KEYDOWN});
    if (running())
        return ev.key.keysym.sym;
    return 0;
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
    (void)f;
    //font = f;
    SDL_SetWindowSize(p_win_, cols_ * font_.w(), rows_ * font_.h());
    redraw();
}

void Term::redraw() {
    bool changed = false;
    for (size_t x = 0; x < cols_; ++x)
        for (size_t y = 0; y < rows_; ++y) {
            if (mask_[y * cols_ + x])
                redraw(x, y), changed = true;
            mask_[y * cols_ + x] = false;
        }
    if (changed) {
        SDL_RenderPresent(p_ren_);
        SDL_Delay(1);
    }
}

void Term::redraw(size_t x, size_t y) {
    std::cout << "Redraw " << x << ' ' << y << get(x, y) << std::endl;
    SDL_Rect dst {static_cast<int>(x * font_.w()),
                  static_cast<int>(y * font_.h()),
                  static_cast<int>(font_.w()),
                  static_cast<int>(font_.h())};
    font_.render(dst, get(x, y), fgCol_, bgCol_);
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
        term->quitRequested_ = true;
        break;
    default:
        break;
    }
    return 1;
}
}
