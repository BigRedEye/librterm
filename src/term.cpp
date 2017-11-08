#include "term.h"
#include "font.h"
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <iostream>

namespace term {
Term::Term()
    : Term(0, 0) {
}

Term::Term(size_t cols, size_t rows)
    : cols_(cols),
      rows_(rows),
      data_(rows * cols, ' '),
      mask_(rows * cols, true),
      cursorPos_(),
      quitRequested_(false),
      fgCol_(0, 255, 0),
      bgCol_(0, 0, 0),
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
    SDL_RenderClear(p_ren_);
    SDL_RenderPresent(p_ren_);
    font_.setRenderer(p_ren_);
    SDL_AddEventWatch(eventFilter, this);
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

void Term::delay(uint32_t msec) const {
    SDL_Delay(msec);
}

Term::Char& Term::get(size_t x, size_t y) {
    return data_[y * cols_ + x];
}

Term::Char Term::get(size_t x, size_t y) const {
    return data_[y * cols_ + x];
}

void Term::resize(size_t rows, size_t cols) {
    /* we need to clear mask vector before resizing */
    redraw();

    /* convert 1d-vector data_ to 2d-vector data2d */
    std::vector<std::vector<Char>> data2d(rows, std::vector<Char>(cols, ' '));
    size_t i = 0;  // current row in data2d
    for (auto it = data_.begin(); it != data_.end(); ++i)
        for (size_t j = 0; j < data2d[i].size(); ++j)
            data2d[i][j] = *++it;

    /* copy data2d to data_ */
    data_.resize(rows * cols);
    mask_.resize(rows * cols, 0);
    auto data_it = data_.begin();
    for (auto y_it = data2d.begin(); y_it != data2d.end(); ++y_it)
        for (auto x_it = y_it->begin(); x_it != y_it->end(); ++x_it)
            *data_it++ = *x_it;

    /* resize window */
    SDL_SetWindowSize(p_win_, cols * font_.w(), rows * font_.h());
    redraw();
}

void Term::setChar(size_t x, size_t y, char_t c) {
    mask_[y * cols_ + x] = true;
    get(x, y).ch_ = c;
}

void Term::addChar(char_t c) {
    switch (c) {
    case '\n':
    case '\r':
        cursorPos_ = (cursorPos_ + cols_ - cursorPos_ % cols_) % data_.size();
        break;
    case '\b':
        cursorPos_ = cursorPos_ ? cursorPos_ - 1: 0;
        data_[cursorPos_].ch_ = ' ';
        mask_[cursorPos_] = true;
        break;
    case '\t':
        for (int i = 0; i < 4; ++i)
            addChar(' ');
        break;
    default:
        mask_[cursorPos_] = true;
        data_[cursorPos_++].ch_ = c;
        cursorPos_ %= data_.size();
    }
}

Key Term::getKey() const {
    SDL_StartTextInput();
    while (true) {
        SDL_Event ev = getNextEvent({SDL_TEXTINPUT, SDL_KEYDOWN});
        if (!running()) {
            SDL_StopTextInput();
            return Key();
        }
        switch (ev.type) {
        case SDL_TEXTINPUT:
            SDL_StopTextInput();
            return Key(UTF8BytesToChar(ev.text.text));
        case SDL_KEYDOWN:
            if (0 /*ev.key.keysym.sym == SDLK_RETURN ||
                ev.key.keysym.sym == SDLK_BACKSPACE ||
                ev.key.keysym.sym == SDLK_TAB*/) {
                SDL_StopTextInput();
                return Key(ev.key.keysym.sym);
            }
            break;
        default:
            break;
        }
    }
    /* SDL_Event ev = getNextEvent({SDL_KEYDOWN, SDL_TEXTINPUT});
    if (running())
        return ev.key.keysym.sym; */
    return 0;
}

char_t Term::getChar() const {
    Key key;
    while (running() && !((key = getKey()).toChar()))
        std::cerr << "getKey()" << std::endl;
    std::cerr << "getChar()" << std::endl;
    return key.toChar();
/*
    SDL_StartTextInput();
    while (true) {
        SDL_Event ev = getNextEvent({SDL_TEXTINPUT, SDL_KEYDOWN});
        if (!running())
            return 0;
        switch (ev.type) {
        case SDL_TEXTINPUT:
            SDL_StopTextInput();
            return UTF8BytesToChar(ev.text.text);
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
*/
}

char_t Term::charAt(size_t x, size_t y) const {
    return get(x, y).ch_;
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

void Term::setBgColor(const Color &bg) {
    bgCol_ = bg;
}

void Term::setBgColor(const Color &bg, size_t x, size_t y) {
    get(x, y).bg_ = bg;
    mask_[y * cols_ + x] = true;
}

void Term::setFgColor(const Color &fg) {
    fgCol_ = fg;
}

void Term::setFgColor(const Color &fg, size_t x, size_t y) {
    get(x, y).fg_ = fg;
    mask_[y * cols_ + x] = true;
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
    SDL_Rect dst {static_cast<int>(x * font_.w()),
                  static_cast<int>(y * font_.h()),
                  static_cast<int>(font_.w()),
                  static_cast<int>(font_.h())};
    Char ch = get(x, y);
    font_.render(dst, UTF8CharToBytes(ch.ch_).c_str(), ch.fg_, ch.bg_);
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
