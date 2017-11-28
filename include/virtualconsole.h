#ifndef RTERM_VIRTUAL_CONSOLE_H
#define RTERM_VIRTUAL_CONSOLE_H

/// @cond INTERNAL

#include <cstddef>
#include <utility>
#include <vector>

#include "char.h"

namespace rterm {
class Char;

class VirtualConsole {
public:
    VirtualConsole();
    VirtualConsole(size_t ncols, size_t nrows);
    
    void resize(size_t ncols, size_t nrows);

    size_t cols() const;
    size_t rows() const;
    Char get(size_t x, size_t y) const;
    void set(size_t x, size_t y, Char c);
    void addChar(Char c);

    std::vector<std::pair<size_t, size_t>> getUpdatedChars(bool force = false);
private:
    std::vector<std::vector<Char>> data_;
    std::vector<std::vector<char>> mask_;
    size_t cursorX,
           cursorY;
};

class Char {
public:
    Char(char_t ch = 0, Color bg = Color(0, 0, 0), Color fg = Color(255, 255, 255))
        : ch_(ch), bg_(bg), fg_(fg) {}
    
    inline char_t c() const {
        return ch_;
    }

    inline Color bg() const {
        return bg_;
    }

    inline Color fg() const { 
        return fg_;
    }
    
    friend class VirtualConsole;
    friend class Term;
private:
    char_t ch_;
    Color bg_, fg_;
};

}

/// @endcond INTERNAL

#endif // RTERM_VIRTUAL_CONSOLE_H
