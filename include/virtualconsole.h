#ifndef RTERM_VIRTUAL_CONSOLE_H
#define RTERM_VIRTUAL_CONSOLE_H

/// @cond INTERNAL

#include <cstddef>
#include <utility>
#include <vector>

#include "char.h"

namespace rterm {
class Char;

/**
 * @brief The VirtualConsole class
 * 
 * This class represents virtual console as 2d-array of characters
 */
class VirtualConsole {
public:
    /**
     * @brief Create empty VirtualConsole
     */
    VirtualConsole();

    /**
     * @brief Create VirtualConsole of fixed size
     * @param ncols number of columns
     * @param nrows number of rows
     */
    VirtualConsole(size_t ncols, size_t nrows);

    /**
     * @brief Resize VirtualConsole
     * @param ncols new number of columns
     * @param nrows new number of rows
     */
    void resize(size_t ncols, size_t nrows);

    /**
     * @brief Gen number of columns
     * @return number of columns
     */
    size_t cols() const;
    
    /**
     * @brief Gen number of rows
     * @return number of rows
     */
    size_t rows() const;

    /**
     * @brief Get char at specific position
     * @param x position
     * @param y position
     * @return Char at (x, y)
     * @warning this function doesn't provide bounds checking
     */
    Char get(size_t x, size_t y) const;

    /**
     * @brief Set Char at specific position
     * @param x position
     * @param y position
     * @param c character
     * @warning this function doesn't provide bounds checking
     */
    void set(size_t x, size_t y, Char c);

    /**
     * @brief Add char at cursor position and move cursor forward
     * @param c character
     */
    void addChar(char_t c);

    /**
     * @brief Get updated positions since last getUpdatedChars call
     * @param force if true, returns whole console
     * @return std::vector of updated positions, where positions are std::pair<size_t, size_t>(x, y)
     */
    std::vector<std::pair<size_t, size_t>> getUpdatedChars(bool force = false);

private:
    std::vector<std::vector<Char>> data_; ///< array of characters
    std::vector<std::vector<char>> mask_; ///< mask_[i][j] = true, if character at (j, i) was uptated
    size_t cursorX, ///< cursor position
           cursorY; ///< cursor position
};

/**
 * @brief The Char class
 * 
 * This class contains UTF8 character and back- and fore- ground colors
 */
class Char {
public:

    /**
     * @brief Constructs Char objects
     * @param ch UTF8 character
     * @param bg background color
     * @param fg foreground color
     */
    Char(char_t ch = 0, Color bg = Color(0, 0, 0), Color fg = Color(255, 255, 255))
        : ch_(ch), bg_(bg), fg_(fg) {}

    /**
     * @brief Get UTF8 character
     * @return UTF8 character
     */
    inline char_t c() const {
        return ch_;
    }

    /**
     * @brief Get background color
     * @return background color
     */
    inline Color bg() const {
        return bg_;
    }

    /**
     * @brief Get foreground color
     * @return foreground color
     */
    inline Color fg() const { 
        return fg_;
    }
    
    friend class VirtualConsole;
    friend class Term;
private:
    char_t ch_; ///< real UTF8 char
    Color bg_, ///< background color
          fg_; ///< foreground color
};

}

/// @endcond INTERNAL

#endif // RTERM_VIRTUAL_CONSOLE_H
