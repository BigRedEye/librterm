#ifndef RTERM_TERM_H
#define RTERM_TERM_H

#include <cstddef>
#include <vector>
#include <set>
#include <SDL2/SDL.h>

#include "sdl_ptr.h"
#include "font.h"
#include "key.h"
#include "color.h"
#include "char.h"
#include "virtualconsole.h"

namespace rterm {

class Term {
public:
    Term();
    Term(size_t cols, size_t rows);
    ~Term();

    bool isRunning() const;
    size_t cols() const;
    size_t rows() const;
    void delay(uint32_t msec) const;

    void resize(size_t cols_, size_t rows_);
    void setWindowSize(size_t width, size_t height);
    void setChar(size_t x, size_t y, char_t c);
    void setFont(const std::string &s, size_t sz);
    void setFont(const std::string &s, size_t w, size_t h);
    void setBgColor(const Color &bg);
    void setBgColor(const Color &bg, size_t x, size_t y);
    void setFgColor(const Color &fg);
    void setFgColor(const Color &fg, size_t x, size_t y);

    void setFullscreen(bool);
    void setResizable(bool);
    void setMinWindowSize(size_t width, size_t height);
    void setMaxWindowSize(size_t width, size_t height);
    void close();

    /* wait for next key */
    Key getKey() const;
    /* wait for next ASCII char */
    char_t getChar() const;

    void addChar(char_t c);

    void redraw(bool force = false);
    void redraw(size_t x, size_t y);

    char_t charAt(size_t x, size_t y) const;
    SDL_Event getNextEvent(const std::set<int> &filter) const;

    friend int eventFilter(void *data_, SDL_Event *ev);

private:
    Char& get(size_t x, size_t y);
    Char get(size_t x, size_t y) const;
    void updateTexture();
    void renderToScreen();

    VirtualConsole console_;
    
    Font *p_font_;
    SDL_Ptr<SDL_Window> p_win_;
    SDL_Ptr<SDL_Renderer> p_ren_;
    SDL_Ptr<SDL_Texture> p_tex_;

    bool quitRequested_;

    Color fgCol_;
    Color bgCol_;
};

int eventFilter(void *data, SDL_Event *ev);
}
#endif // RTERM_TERM_H
