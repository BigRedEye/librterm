#include "term.h"
#include "font.h"
#include "ttfont.h"
#include "tilefont.h"
#include <iostream>

#ifdef RTERM_DEBUG
#include <iostream>
#include <chrono>
#endif // RTERM_DEBUG

namespace rterm {
Term::Term()
    : Term(0, 0) {
}

Term::Term(size_t ncols, size_t nrows)
    : console_(ncols, nrows),
      quitRequested_(false),
      fgCol_(0, 255, 0),
      bgCol_(0, 0, 0),
      p_font_(new TTFont()) {
    SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    TTF_Init();

    p_win_ = SDL_Ptr<SDL_Window>(SDL_CreateWindow("Terminal", SDL_WINDOWPOS_UNDEFINED,
                                         SDL_WINDOWPOS_UNDEFINED,
                                         p_font_->w() * cols(), p_font_->h() * rows(), 0));
    p_ren_ = SDL_Ptr<SDL_Renderer>(SDL_CreateRenderer(p_win_.get(), -1, 0));
    
    SDL_RenderClear(p_ren_.get());
    SDL_RenderPresent(p_ren_.get());
    SDL_AddEventWatch(eventFilter, this);
    
    SDL_RenderClear(p_ren_.get());
    SDL_RenderPresent(p_ren_.get());
    p_tex_ = SDL_Ptr<SDL_Texture>(SDL_CreateTexture(p_ren_.get(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
                                  SDL_GetWindowSurface(p_win_.get())->w, SDL_GetWindowSurface(p_win_.get())->h));
    redraw();
}

Term::~Term() {
    delete p_font_;

    TTF_Quit();
    SDL_Quit();
}

size_t Term::cols() const {
    return console_.cols();
}

size_t Term::rows() const {
    return console_.rows();
}

bool Term::isRunning() const {
    return !quitRequested_;
}

void Term::delay(uint32_t msec) const {
    SDL_Delay(msec);
}

void Term::updateTexture() {
    SDL_Texture * tmp = p_tex_.get();

    p_tex_ = SDL_Ptr<SDL_Texture>(SDL_CreateTexture(p_ren_.get(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
                               SDL_GetWindowSurface(p_win_.get())->w - SDL_GetWindowSurface(p_win_.get())->w % p_font_->w(),
                               SDL_GetWindowSurface(p_win_.get())->h - SDL_GetWindowSurface(p_win_.get())->h % p_font_->h()));

    SDL_SetRenderTarget(p_ren_.get(), p_tex_.get());
    SDL_SetRenderDrawColor(p_ren_.get(), bgCol_.r(), bgCol_.g(), bgCol_.b(), bgCol_.a());
    SDL_Rect dstRect{0, 0, 0, 0};
    SDL_QueryTexture(p_tex_.get(), NULL, NULL, &dstRect.w, &dstRect.h);
    SDL_RenderCopy(p_ren_.get(), tmp, NULL, &dstRect);
    SDL_DestroyTexture(tmp);
    SDL_SetRenderTarget(p_ren_.get(), NULL);
}

void Term::setWindowSize(size_t width, size_t height) {    
    size_t ncols = width / p_font_->w(),
           nrows = height / p_font_->h();
    console_.resize(ncols, nrows);

    /* resize window */
    SDL_SetWindowSize(p_win_.get(), width, height);
    updateTexture();

    SDL_RenderClear(p_ren_.get());
    redraw(true);
}

void Term::resize(size_t ncols, size_t nrows) {
    setWindowSize(ncols * p_font_->w(), nrows * p_font_->h());
}

void Term::setChar(size_t x, size_t y, char_t c) {
    console_.set(x, y, Char(c, bgCol_, fgCol_));
}

void Term::addChar(char_t c) {
    console_.addChar(c);
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
    return console_.get(x, y).c();
}

Color Term::bgColorAt(size_t x, size_t y) const {
    return console_.get(x, y).bg();
}

Color Term::fgColorAt(size_t x, size_t y) const {
    return console_.get(x, y).fg();
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

    SDL_SetWindowFullscreen(p_win_.get(), (fullscr ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0));

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

void Term::close() {

}

void Term::setFont(const std::string &path, size_t sz) {
    if (p_font_)
        delete p_font_;

    p_font_ = new TTFont(path, sz);
    setWindowSize(p_font_->w() * cols(), p_font_->h() * rows());
    redraw(true);
}

void Term::setFont(const std::string &path, size_t w, size_t h) {
    if (p_font_)
        delete p_font_;

    p_font_ = new TileFont(path, w, h);
    setWindowSize(p_font_->w() * cols(), p_font_->h() * rows());
    redraw(true);
}

void Term::setBgColor(const Color &bg) {
    bgCol_ = bg;
    for (int i = 0; i < rows(); ++i)
        for (int j = 0; j < cols(); ++j)
            console_.set(j, i, Char(console_.get(j, i).c(),
                                    bg,
                                    console_.get(j, i).fg()));
}

void Term::setBgColor(const Color &bg, size_t x, size_t y) {
    console_.set(x, y, Char(console_.get(x, y).c(),
                            bg,
                            console_.get(x, y).fg()));
}

void Term::setFgColor(const Color &fg) {
    fgCol_ = fg;
    for (int i = 0; i < rows(); ++i)
        for (int j = 0; j < cols(); ++j)
            console_.set(j, i, Char(console_.get(j, i).c(),
                                    console_.get(j, i).bg(),
                                    fg));
}

void Term::setFgColor(const Color &fg, size_t x, size_t y) {
    console_.set(x, y, Char(console_.get(x, y).c(),
                            console_.get(x, y).bg(), 
                            fg));
}

void Term::redraw(bool force) {
#ifdef RTERM_DEBUG
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
#endif // RTERM_DEBUG

    auto update = console_.getUpdatedChars(force);
    for (auto p : update)
        redraw(p.first, p.second);
    if (!update.empty() || force)
        renderToScreen();
#ifdef RTERM_DEBUG
    static int64_t frame = 0;
    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> deltaTime = end - start;
    std::cerr << "frame " << ++frame << ", deltaTime: " << deltaTime.count() << " msec" << std::endl;
#endif // RTERM_DEBUG
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
    Char ch = console_.get(x, y);
    SDL_SetRenderTarget(p_ren_.get(), p_tex_.get());
    p_font_->render(p_ren_.get(), dst, ch.ch_, ch.fg_, ch.bg_);
    SDL_SetRenderTarget(p_ren_.get(), NULL);
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
