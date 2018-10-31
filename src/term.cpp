#include "term.h"
#include "font.h"
#include "hardware_texture.h"
#include "logger.h"
#include "mouse.h"
#include "tilefont.h"
#include "ttfont.h"

#include <SDL_image.h>

#include <functional>
#include <iostream>
#include <sstream>
#include <thread>

#define UNUSED(var) (void)var;

namespace rterm {
using namespace events;

Term::Term()
    : Term(0, 0) {
}

Term::Term(const TermFormat& format)
    : console_(format.w(), format.h())
    , window_(400, 400)
    , glyphCache_(window_.renderer(), std::make_unique<TTFont>())
    , quitRequested_(false)
    , fgCol_(0x00, 0xff, 0x00)
    , bgCol_(0x00, 0x00, 0x00) {
    redraw(true);
    setupCallbacks();
    eventSystem_.poll();
}

Term::~Term() {
    quitRequested_ = true;
}

size_t Term::cols() const {
    return console_.cols();
}

size_t Term::rows() const {
    return console_.rows();
}

void Term::poll() {
    eventSystem_.poll();
}

bool Term::isRunning() const {
    return !quitRequested_;
}

void Term::delay(uint32_t msec) {
    auto until = std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(msec);
    while (std::chrono::high_resolution_clock::now() < until) {
        poll();
        if (!isRunning()) {
            return;
        }
        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
}

long double Term::fps() const {
    return frameRateCounter_.getFps();
}

size_t Term::getCursorX() const {
    return console_.cursorX();
}

size_t Term::getCursorY() const {
    return console_.cursorY();
}

void Term::setCursorPosition(size_t x, size_t y) {
    console_.setCursorPosition(x, y);
}

void Term::setWindowSize(size_t width, size_t height) {
    size_t ncols = width / glyphCache_.w();
    size_t nrows = height / glyphCache_.h();
    console_.resize(ncols, nrows, bgCol_, fgCol_);

    /* resize window */
    window_.resize(width, height);
    redraw(true);
}

Term& Term::setTitle(const std::string& title) {
    window_.setTitle(title);
    return *this;
}

Term& Term::setIcon(const std::string& path) {
    SoftwareTexture pIcon(IMG_Load(path.c_str()));
    if (!pIcon) {
        throw Exception();
    } else {
        window_.setIcon(pIcon);
        SDL_SetWindowIcon(window_.get(), pIcon.get());
    }
    return *this;
}

void Term::resize(size_t ncols, size_t nrows) {
    setWindowSize(ncols * glyphCache_.w(), nrows * glyphCache_.h());
}

void Term::setChar(size_t x, size_t y, ch32 c) {
    console_.set(x, y, Char(c, bgCol_, fgCol_));
}

void Term::addChar(ch32 c) {
    console_.addChar(c);
}

void Term::print(size_t x, size_t y, const std::string& str) {
    std::u32string u32formatted = utf8ToUtf32(str);
    size_t prevCursorX = getCursorX();
    size_t prevCursorY = getCursorY();
    setCursorPosition(x, y);
    for (ch32 c : u32formatted) {
        addChar(c);
    }
    setCursorPosition(prevCursorX, prevCursorY);
}

Key Term::getKey() {
    return eventSystem_.getKey();
}

ch32 Term::getChar() {
    return eventSystem_.getChar();
}

void Term::getMousePosition(size_t& x, size_t& y) {
    int mx = 0;
    int my = 0;
    SDL_GetMouseState(&mx, &my);
    x = mx / glyphCache_.w();
    y = my / glyphCache_.h();
    x = std::max(std::min(x, cols() - size_t(1)), size_t(0ul));
    y = std::max(std::min(y, rows() - size_t(1)), size_t(0ul));
}

int Term::getMouseButtons() {
    int result = SDL_GetMouseState(NULL, NULL);
    return result;
}

ch32 Term::charAt(size_t x, size_t y) const {
    return console_.get(x, y).c();
}

Color Term::bgColorAt(size_t x, size_t y) const {
    return console_.get(x, y).bg();
}

Color Term::fgColorAt(size_t x, size_t y) const {
    return console_.get(x, y).fg();
}

SoftwareTexture Term::dumpScreen() const {
    return window_.dumpPixels();
}

std::string Term::getError() const {
    std::string error = SDL_GetError();
    SDL_ClearError();
    return error;
}

void Term::setFullscreen(bool fullscr) {
    int ncols;
    int nrows;
    static bool isFullscr = false;
    static SDL_DisplayMode* windowedMode = nullptr;
    static int prevCols = 0;
    static int prevRows = 0;

    if (!isFullscr) {
        if (windowedMode == nullptr) {
            windowedMode = new SDL_DisplayMode;
            if (SDL_GetWindowDisplayMode(window_.get(), windowedMode) < 0) {
                throw Exception();
            }
        }
        prevCols = cols(), prevRows = rows();
    }

    if (isFullscr == fullscr) {
        return;
    }

    SDL_SetWindowFullscreen(window_.get(), (fullscr ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0));

    /* set correct resolution */
    ncols = prevCols, nrows = prevRows;
    SDL_DisplayMode mode;
    if (fullscr) {
        if (SDL_GetDesktopDisplayMode(0, &mode) < 0) {
            throw Exception();
        }
        ncols = mode.w / glyphCache_.w();
        nrows = mode.h / glyphCache_.h();
    } else {
        mode = *windowedMode;
    }
    if (SDL_SetWindowDisplayMode(window_.get(), &mode) < 0) {
        throw Exception();
    }

    isFullscr = !isFullscr;
    resize(ncols, nrows);
    redraw(true);
}

void Term::setResizable(bool resizable) {
#if SDL_VERSION_ATLEAST(2, 0, 5)
    SDL_SetWindowResizable(window_.get(), (resizable ? SDL_TRUE : SDL_FALSE));
#else
    UNUSED(resizable);
    Logger(Logger::ERROR)
        .printf(
            "SDL version %d.%d.%d doesn't support setWindowResizable, update "
            "it to 2.0.5",
            SDL_MAJOR_VERSION,
            SDL_MINOR_VERSION,
            SDL_PATCHLEVEL);
#endif
}

void Term::setMinWindowSize(size_t width, size_t height) {
    window_.setMinimumSize(width, height);
}

void Term::setMaxWindowSize(size_t width, size_t height) {
    window_.setMaximumSize(width, height);
}

void Term::close() {
    quitRequested_ = true;
}

void Term::setFont(const std::string& path, size_t sz) {
    glyphCache_.setFont(path, sz);
    setWindowSize(glyphCache_.w() * cols(), glyphCache_.h() * rows());
    redraw(true);
}

void Term::setFont(const std::string& path, size_t w, size_t h) {
    glyphCache_.setFont(path, w, h);
    setWindowSize(glyphCache_.w() * cols(), glyphCache_.h() * rows());
    redraw(true);
}

void Term::setBgColor(const Color& bg) {
    bgCol_ = bg;
    for (size_t i = 0; i < rows(); ++i) {
        for (size_t j = 0; j < cols(); ++j) {
            Char toSet(console_.get(j, i).c(), bg, console_.get(j, i).fg());
            console_.set(j, i, toSet);
        }
    }
    redraw();
}

void Term::setBgColor(const Color& bg, size_t x, size_t y) {
    Char toSet(console_.get(x, y).c(), bg, console_.get(x, y).fg());
    console_.set(x, y, toSet);
}

void Term::setFgColor(const Color& fg) {
    fgCol_ = fg;
    for (size_t i = 0; i < rows(); ++i) {
        for (size_t j = 0; j < cols(); ++j) {
            Char toSet(console_.get(j, i).c(), console_.get(j, i).bg(), fg);
            console_.set(j, i, toSet);
        }
    }
    redraw();
}

void Term::setFgColor(const Color& fg, size_t x, size_t y) {
    Char toSet(console_.get(x, y).c(), console_.get(x, y).bg(), fg);
    console_.set(x, y, toSet);
}

void Term::redraw(bool force) {
    auto update = console_.getUpdatedChars(force);
    redrawBackground(update);
    redrawForeground(update);
    if (!update.empty() || force) {
        renderToScreen();
    }
}

void Term::renderToScreen() {
    window_.renderer().flush();
    frameRateCounter_.nextFrame(highResClock::now());
}

void Term::setupCallbacks() {
    eventSystem_.registerCallback(EventType::Quit, [this](Event* ev) {
        UNUSED(ev);
        this->close();
    });
    eventSystem_.registerCallback(EventType::WindowExposed, [this](Event* ev) {
        UNUSED(ev);
        this->redraw(true);
    });
    eventSystem_.registerCallback(EventType::WindowResized, [this](Event* ev) {
        auto event = static_cast<events::WindowResizedEvent*>(ev);
        this->setWindowSize(event->x(), event->y());
        this->redraw(true);
    });
}

void Term::redrawGlyph(size_t x, size_t y) {
    ScreenView dst(x * glyphCache_.w(), y * glyphCache_.h(), glyphCache_.w(), glyphCache_.h());
    Char ch = console_.get(x, y);
    TextureView<Api::api> tview = glyphCache_.getGlyph(ch.c());
    window_.renderer().render(tview, dst, ch.fg());
}

void Term::redrawBackground(const std::vector<std::pair<ui32, ui32>>& updates) {
    auto hash = [](Color color) { return std::hash<ui32>{}(color.rgba()); };
    std::unordered_map<Color, std::vector<ScreenView>, decltype(hash)> colors(0, hash);
    for (const auto& p : updates) {
        ScreenView dst(p.first * glyphCache_.w(), p.second * glyphCache_.h(), glyphCache_.w(), glyphCache_.h());
        colors[console_.get(p.first, p.second).bg()].push_back(std::move(dst));
    }
    for (const auto& color : colors) {
        window_.renderer().fillRects(color.first, color.second);
    }
}

void Term::redrawForeground(const std::vector<std::pair<ui32, ui32>>& updates) {
    for (const auto& p : updates) {
        redrawGlyph(p.first, p.second);
    }
}

} // namespace rterm
