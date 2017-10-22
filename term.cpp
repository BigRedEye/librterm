#include "term.h"
#include <SDL2/SDL.h>

namespace term {
Term::Term()
    : Term(0, 0) {
}

Term::Term(size_t rows, size_t cols)
    : cols(cols),
      rows(rows),
      font(),
      data(rows * cols, ' '),
      mask(rows * cols, false) {
    SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    SDL_CreateWindowAndRenderer(cols * fontSize, rows * fontSize, 0, p_win, p_ren);
}

Term::~Term() {
    SDL_DestroyRenderer(p_ren);
    SDL_DestroyWindow(p_win);
    SDL_Quit();
}

void Term::resize(size_t _rows, size_t _cols) {
}

void Term::setChar(size_t x, size_t y, char c) {
    mask(y * cols + x) = true;
    get(x, y) = c;
}

char Term::getChar(size_t x, size_t y) {
    return get(x, y);
}

void Term::setFullscreen() {
}

void Term::setFont(const Font &f) {
    font = f;
    SDL_SetWindowSize(p_win, cols * f.w(), rows * f.h());
    redraw();
}

void Term::redraw() {
    for (size_t x = 0; x < cols; ++x)
        for (size_t y = 0; y < rows; ++y) {
            if (mask[y * cols + x])
                redraw(x, y);
            mask[y * cols + x] = false;
        }
}

void Term::redraw(size_t x, size_t y) {
    SDL_Rect rect {x, y, font.w(), font.h()};
    SDL_RenderCopy(p_ren, font.getTexture(), font.getRect(get(x, y)), &rect);
}

void Term::getNextEvent() {
    SDL_Event ev;
    if (SDL_PollEvent(&ev))
        return Event(&ev);
    return Event(nullptr);
}
}
