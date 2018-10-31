#pragma once

/// @cond INTERNAL

#include "char.h"
#include "color.h"

#include <cstddef>
#include <utility>
#include <vector>

namespace rterm {

/**
 * @brief The Char class
 *
 * This class contains UTF32 character and back- and fore- ground colors
 */
class Char {
public:
    /**
     * @brief Constructs Char objects
     * @param ch UTF32 character
     * @param bg background color
     * @param fg foreground color
     */
    Char(ch32 ch = 0, Color bg = Color::Black, Color fg = Color::White)
        : ch_(ch)
        , bg_(bg)
        , fg_(fg) {
    }

    /**
     * @brief eq operator
     * @param other
     * @return @code
     * this->c() == other.c() &&
     * this->bg() == other.bg() &&
     * this->fg() == other.fg() @endcode
     */
    bool operator==(const Char& other) const;

    /**
     * @brief neq operator
     * @param other
     * @return @code !(*this == other) @endcode
     */
    bool operator!=(const Char& other) const;

    /**
     * @brief Get UTF8 character
     * @return UTF8 character
     */
    inline ch32 c() const {
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
    ch32 ch_;  ///< real UTF8 char
    Color bg_; ///< background color
    Color fg_; ///< foreground color
};

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
    VirtualConsole(ui32 ncols, ui32 nrows);

    /**
     * @brief Resize VirtualConsole
     * @param ncols new number of columns
     * @param nrows new number of rows
     * @param bgColor default background color
     * @param fgColor default foreground color
     */
    void resize(ui32 ncols, ui32 nrows, Color bgColor, Color fgColor);

    /**
     * @brief Gen number of columns
     * @return number of columns
     */
    ui32 cols() const;

    /**
     * @brief Gen number of rows
     * @return number of rows
     */
    ui32 rows() const;

    /**
     * @brief Get char at specific position
     * @param x position
     * @param y position
     * @return Char at (x, y)
     * @warning this function doesn't provide bounds checking
     */
    const Char& get(ui32 x, ui32 y) const;

    /**
     * @brief Set Char at specific position
     * @param x position
     * @param y position
     * @param c character
     * @warning this function doesn't provide bounds checking
     */
    void set(ui32 x, ui32 y, const Char& c);

    /**
     * @brief Cursor X position
     * @return Cursor X position
     */
    ui32 cursorX() const;

    /**
     * @brief Cursor Y position
     * @return Cursor Y position
     */
    ui32 cursorY() const;

    /**
     * @brief Set cursor position
     * @param x new x position
     * @param y new y position
     */
    void setCursorPosition(ui32 x, ui32 y);

    /**
     * @brief Add char at cursor position and move cursor forward
     * @param c character
     */
    void addChar(ui32 c);

    /**
     * @brief Get updated positions since last getUpdatedChars call
     * @param force if true, returns whole console
     * @return std::vector of updated positions, where positions are
     * std::pair<size_t, size_t>(x, y)
     */
    std::vector<std::pair<ui32, ui32>> getUpdatedChars(bool force = false);

private:
    bool getMask(ui32 x, ui32 y) const;

private:
    static const Char space_;

    std::vector<std::vector<Char>> data_; ///< array of characters
    std::vector<std::vector<char>> mask_;
    ///< mask_[i][j] = true, if character at (j, i) was uptated
    ui32 cursorX_; ///< cursor position
    ui32 cursorY_; ///< cursor position
};

} // namespace rterm

/// @endcond INTERNAL
