#include "term.h"
#include "font.h"
#include <SDL2/SDL_ttf.h>
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
    TTF_Init();

    p_win_ = SDL_CreateWindow("Terminal", SDL_WINDOWPOS_UNDEFINED,
                                         SDL_WINDOWPOS_UNDEFINED,
                                         cols_ * 10, rows_ * 10, 0);
    p_ren_ = SDL_CreateRenderer(p_win_, 0, 0);
    font_ = Font(p_ren_, "DejaVuSansMono.ttf", 18);
    SDL_SetWindowSize(p_win_, font_.w() * cols_, font_.h() * rows_);
    SDL_RenderClear(p_ren_);
    SDL_RenderPresent(p_ren_);
    font_.setRenderer(p_ren_);
    SDL_AddEventWatch(eventFilter, this);
    SDL_RenderFillRect(p_ren_, NULL);
    SDL_SetWindowBordered(p_win_, SDL_FALSE);
    redraw();
}

Term::~Term() {
    font_.destroyFont();

    SDL_DestroyRenderer(p_ren_);
    SDL_DestroyWindow(p_win_);

    TTF_Quit();
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

void Term::resize(size_t cols, size_t rows) {
    /* we need to clear mask vector before resizing */
    redraw();

    /* convert 1d-vector data_ to 2d-vector data2d */
    std::vector<std::vector<Char>> data2d(cols, std::vector<Char>(rows, ' '));
    size_t i = 0;  // current row in data2d
    auto it = data_.begin();
    for (int i = 0; i < std::min(rows_, rows); ++i)
        for (int j = 0; j < std::min(rows_, rows); ++j)
            data2d[i][j] = get(j, i);
    /* copy data2d to data_ */
    data_.resize(rows * cols);
    mask_.assign(rows * cols, true);
    auto data_it = data_.begin();
    for (auto y_it = data2d.begin(); y_it != data2d.end(); ++y_it)
        for (auto x_it = y_it->begin(); x_it != y_it->end(); ++x_it)
            *data_it++ = *x_it;

    /* resize window */
    SDL_SetWindowSize(p_win_, cols * font_.w(), rows * font_.h());
    cols_ = cols;
    rows_ = rows;
    SDL_RenderClear(p_ren_);
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
    while (running()) {
        SDL_Event evs[2];
        SDL_PumpEvents();
        SDL_WaitEvent(NULL);
        int sz = 0;
        if (SDL_PeepEvents(&evs[sz], 1, SDL_GETEVENT, SDL_KEYDOWN, SDL_KEYDOWN) > 0)
            ++sz;
        if (SDL_PeepEvents(&evs[sz], 1, SDL_GETEVENT, SDL_TEXTINPUT, SDL_TEXTINPUT) > 0)
            ++sz;
        Key result;
        for (int i = 0; i < sz; ++i)
            switch (evs[i].type) {
            case SDL_KEYDOWN:
                result.setKey(evs[i].key.keysym.sym)
                      .addMod(evs[i].key.keysym.mod);
                break;
            case SDL_TEXTINPUT:
                result.setChar(UTF8BytesToChar(evs[i].text.text));
                break;
            default:
                break;
            }
            if (result.key() || result.toChar()) {
                return result;
        }
    }
    return Key();
}

char_t Term::getChar() const {
    Key key;
    while (running() && !((key = getKey()).toChar())) {
    }
    return key.toChar();
}

char_t Term::charAt(size_t x, size_t y) const {
    return get(x, y).ch_;
}

void Term::setFullscreen(bool fullscr) {
    static bool isFullscr = false;
    static SDL_DisplayMode *windowedMode = NULL;

    if (windowedMode == NULL && !isFullscr) {
        windowedMode = new SDL_DisplayMode;
        SDL_GetWindowDisplayMode(p_win_, windowedMode);
    }

    if (isFullscr == fullscr)
        return;

    SDL_SetWindowFullscreen(p_win_, (fullscr ? SDL_WINDOW_FULLSCREEN : 0));

    /* set correct resolution */
    SDL_DisplayMode mode;
    if (fullscr)
        SDL_GetDesktopDisplayMode(0, &mode);
    else
        mode = *windowedMode;
    SDL_SetWindowDisplayMode(p_win_, &mode);

    int ncols = mode.w / font_.w();
    int nrows = mode.h / font_.h();
    isFullscr = !isFullscr;
    resize(ncols, nrows);
}

void Term::setFont(const std::string &path, size_t sz) {
    font_ = Font(p_ren_, path, sz);
    SDL_SetWindowSize(p_win_, cols_ * font_.w(), rows_ * font_.h());
    redraw();
}

void Term::setBgColor(const Color &bg) {
    for (int i = 0; i < rows_; ++i)
        for (int j = 0; j < cols_; ++j)
            get(j, i).bg_ = bg, mask_[i * cols_ + j] = true;
}

void Term::setBgColor(const Color &bg, size_t x, size_t y) {
    get(x, y).bg_ = bg;
    mask_[y * cols_ + x] = true;
}

void Term::setFgColor(const Color &fg) {
    for (int i = 0; i < rows_; ++i)
        for (int j = 0; j < cols_; ++j)
            get(j, i).fg_ = fg, mask_[i * cols_ + j] = true;
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
