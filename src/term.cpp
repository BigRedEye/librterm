#include "term.h"
#include "font.h"
#include "ttfont.h"
#include "tilefont.h"
#include <iostream>

#ifdef LIBTERM_DEBUG
#include <iostream>
#include <chrono>
#endif // LIBTERM_DEBUG

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
      p_font_(new TTFont()) {
    std::cerr << "Term::Term starts" << std::endl;
    SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    TTF_Init();

    p_win_ = SDL_Ptr<SDL_Window>(SDL_CreateWindow("Terminal", SDL_WINDOWPOS_UNDEFINED,
                                         SDL_WINDOWPOS_UNDEFINED,
                                         p_font_->w() * cols_, p_font_->h() * rows_, 0));
    p_ren_ = SDL_Ptr<SDL_Renderer>(SDL_CreateRenderer(p_win_.get(), -1, 0));
    
    SDL_RenderClear(p_ren_.get());
    SDL_RenderPresent(p_ren_.get());
    SDL_AddEventWatch(eventFilter, this);
    
    SDL_RenderClear(p_ren_.get());
    SDL_RenderPresent(p_ren_.get());
    p_tex_ = SDL_Ptr<SDL_Texture>(SDL_CreateTexture(p_ren_.get(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
                                  SDL_GetWindowSurface(p_win_.get())->w, SDL_GetWindowSurface(p_win_.get())->h));
    redraw();
    std::cerr << "Term::Term ends" << std::endl;
}

Term::~Term() {
    delete p_font_;

    TTF_Quit();
    SDL_Quit();
}

size_t Term::cols() const {
    return cols_;
}

size_t Term::rows() const {
    return rows_;
}

bool Term::isRunning() const {
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

void Term::updateTexture() {
    SDL_Texture * tmp = p_tex_.get();

    p_tex_ = SDL_Ptr<SDL_Texture>(SDL_CreateTexture(p_ren_.get(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
                               SDL_GetWindowSurface(p_win_.get())->w - SDL_GetWindowSurface(p_win_.get())->w % p_font_->w(),
                               SDL_GetWindowSurface(p_win_.get())->h - SDL_GetWindowSurface(p_win_.get())->h % p_font_->h()));

    SDL_SetRenderTarget(p_ren_.get(), p_tex_.get());
    SDL_Rect dstRect{0, 0, 0, 0};
    SDL_QueryTexture(p_tex_.get(), NULL, NULL, &dstRect.w, &dstRect.h);
    SDL_RenderCopy(p_ren_.get(), tmp, NULL, &dstRect);
    SDL_DestroyTexture(tmp);
    SDL_SetRenderTarget(p_ren_.get(), NULL);
}

void Term::setWindowSize(size_t width, size_t height) {    
    size_t ncols = width / p_font_->w(),
           nrows = height / p_font_->h();
    /* we need to clear mask vector before resizing */
    redraw();

    /* convert 1d-vector data_ to 2d-vector data2d */
    std::vector<std::vector<Char>> data2d(ncols, std::vector<Char>(nrows, Char(' ', bgCol_, fgCol_)));
    size_t i = 0;  // current row in data2d
    auto it = data_.begin();
    for (int i = 0; i < std::min(cols_, ncols); ++i)
        for (int j = 0; j < std::min(rows_, nrows); ++j)
            data2d[i][j] = get(i, j);
    /* copy data2d to data_ */
    data_.resize(nrows * ncols);
    mask_.assign(nrows * ncols, true);
    for (int i = 0; i < ncols; ++i)
        for (int j = 0; j < nrows; ++j)
            data_[j * ncols + i] = data2d[i][j];

    /* resize window */
    SDL_SetWindowSize(p_win_.get(), width, height);
    updateTexture();
    
    int cursorX = cursorPos_ % cols_;
    int cursorY = cursorPos_ / cols_;
    cursorPos_ = cursorY * ncols + cursorX;
    cols_ = ncols;
    rows_ = nrows;
    SDL_RenderClear(p_ren_.get());
    redraw();
}

void Term::resize(size_t ncols, size_t nrows) {
    setWindowSize(ncols * p_font_->w(), nrows * p_font_->h());
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
    while (isRunning()) {
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
    while (isRunning() && !((key = getKey()).toChar())) {
    }
    return key.toChar();
}

char_t Term::charAt(size_t x, size_t y) const {
    return get(x, y).ch_;
}

void Term::setFullscreen(bool fullscr) {
    static bool isFullscr = false;
    static SDL_DisplayMode *windowedMode = NULL;
    static int prevCols = 0, prevRows = 0;

    if (!isFullscr) {
        if (windowedMode == NULL) {
            windowedMode = new SDL_DisplayMode;
            SDL_GetWindowDisplayMode(p_win_.get(), windowedMode);
        }
        prevCols = cols(), prevRows = rows();
    }

    if (isFullscr == fullscr)
        return;

    SDL_SetWindowFullscreen(p_win_.get(), (fullscr ? SDL_WINDOW_FULLSCREEN : 0));

    /* set correct resolution */
    int ncols = prevCols, nrows = prevRows;
    SDL_DisplayMode mode;
    if (fullscr) {
        SDL_GetDesktopDisplayMode(0, &mode);
        ncols = mode.w / p_font_->w();
        nrows = mode.h / p_font_->h();
    }
    else
        mode = *windowedMode;
    SDL_SetWindowDisplayMode(p_win_.get(), &mode);

    isFullscr = !isFullscr;
    resize(ncols, nrows);
}

void Term::setResizable(bool resizable) {
#if SDL_MAJOR_VERSION >= 2 && SDL_PATCHLEVEL >= 5
    SDL_SetWindowResizable(p_win_.get(), (resizable ? SDL_TRUE : SDL_FALSE));  
#else
    SDL_Log("SDL version %d.%d.%d doesn't support setWindowResizable, update it to 2.0.5", 
             SDL_MAJOR_VERSION,
             SDL_MINOR_VERSION,
             SDL_PATCHLEVEL);
#endif
}

void Term::setMinWindowSize(size_t width, size_t height) {
    SDL_SetWindowMinimumSize(p_win_.get(), width, height);
}

void Term::setMaxWindowSize(size_t width, size_t height) {
    SDL_SetWindowMaximumSize(p_win_.get(), width, height);
}

void Term::setFont(const std::string &path, size_t sz) {
    if (p_font_)
        delete p_font_;

    p_font_ = new TTFont(path, sz);
    resize(cols_, rows_);
    redraw();
}

void Term::setFont(const std::string &path, size_t w, size_t h) {
    if (p_font_)
        delete p_font_;

    p_font_ = new TileFont(path, w, h);
    resize(cols_, rows_);
    redraw();
}

void Term::setBgColor(const Color &bg) {
    bgCol_ = bg;
    for (int i = 0; i < rows_; ++i)
        for (int j = 0; j < cols_; ++j)
            get(j, i).bg_ = bg, mask_[i * cols_ + j] = true;
}

void Term::setBgColor(const Color &bg, size_t x, size_t y) {
    get(x, y).bg_ = bg;
    mask_[y * cols_ + x] = true;
}

void Term::setFgColor(const Color &fg) {
    fgCol_ = fg;
    for (int i = 0; i < rows_; ++i)
        for (int j = 0; j < cols_; ++j)
            get(j, i).fg_ = fg, mask_[i * cols_ + j] = true;
}

void Term::setFgColor(const Color &fg, size_t x, size_t y) {
    get(x, y).fg_ = fg;
    mask_[y * cols_ + x] = true;
}

void Term::redraw() {
#ifdef LIBTERM_DEBUG
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
#endif // LIBTERM_DEBUG

    bool changed = true;
    for (size_t x = 0; x < cols_; ++x)
        for (size_t y = 0; y < rows_; ++y) {
            if (mask_[y * cols_ + x])
                redraw(x, y), changed = true;
            mask_[y * cols_ + x] = false;
        }
    if (changed) {
        renderToScreen();
    }
#ifdef LIBTERM_DEBUG
    static int64_t frame = 0;
    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> deltaTime = end - start;
    std::cerr << "frame " << ++frame << ", deltaTime: " << deltaTime.count() << " msec" << std::endl;
#endif // LIBTERM_DEBUG
}

void Term::renderToScreen() {
    SDL_Rect textureRect{0, 0, 0, 0};
    SDL_Rect windowRect{0, 0, 0, 0};
    SDL_QueryTexture(p_tex_.get(), NULL, NULL, &textureRect.w, &textureRect.h);
    SDL_GetWindowSize(p_win_.get(), &windowRect.w, &windowRect.h);
    SDL_Rect dstRect{0, 0, std::min(textureRect.w, windowRect.w), 
                           std::min(textureRect.h, windowRect.h)};
    SDL_RenderClear(p_ren_.get());
    SDL_RenderCopy(p_ren_.get(), p_tex_.get(), &dstRect, &dstRect);
    SDL_RenderPresent(p_ren_.get());
}

void Term::redraw(size_t x, size_t y) {
    SDL_Rect dst {static_cast<int>(x * p_font_->w()),
                  static_cast<int>(y * p_font_->h()),
                  static_cast<int>(p_font_->w()),
                  static_cast<int>(p_font_->h())};
    Char ch = get(x, y);
    SDL_SetRenderTarget(p_ren_.get(), p_tex_.get());
    p_font_->render(p_ren_.get(), dst, UTF8CharToBytes(ch.ch_).c_str(), ch.fg_, ch.bg_);
    SDL_SetRenderTarget(p_ren_.get(), NULL);
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
    case SDL_WINDOWEVENT:
        switch (ev->window.event) {
        case SDL_WINDOWEVENT_RESIZED:
            term->setWindowSize(ev->window.data1, ev->window.data2);
            break;
        case SDL_WINDOWEVENT_EXPOSED:
            term->renderToScreen();
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
    return 1;
}
}
