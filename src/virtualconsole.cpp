#include "rterm/virtualconsole.h"
#include "rterm/error.h"

#include <sstream>

namespace rterm {

const Char VirtualConsole::space_{' '};

VirtualConsole::VirtualConsole()
    : VirtualConsole(0, 0) {
}

VirtualConsole::VirtualConsole(ui32 ncols, ui32 nrows)
    : cursorX_(0)
    , cursorY_(0) {
    data_.assign(nrows, std::vector<Char>(ncols, ' '));
    mask_.assign(nrows, std::vector<char>(ncols, false));
}

ui32 VirtualConsole::cols() const {
    if (data_.empty()) {
        return 0;
    }
    return data_[0].size();
}

ui32 VirtualConsole::rows() const {
    return data_.size();
}

void VirtualConsole::resize(ui32 ncols, ui32 nrows, Color bgColor, Color fgColor) {
    Char defaultChar(' ', bgColor, fgColor);
    data_.resize(nrows, std::vector<Char>(ncols, defaultChar));
    mask_.resize(nrows, std::vector<char>(ncols, true));
    for (auto& line : data_) {
        line.resize(ncols, defaultChar);
    }

    for (auto& line : mask_) {
        line.resize(ncols, true);
    }

    cursorX_ = std::min(cursorX_, ncols);
    cursorY_ = std::min(cursorY_, nrows);
}

const Char& VirtualConsole::get(ui32 x, ui32 y) const {
    if (x < cols() && y < rows()) {
        return data_[y][x];
    }
    return space_;
}

bool VirtualConsole::getMask(ui32 x, ui32 y) const {
    if (x < cols() && y < rows()) {
        return mask_[y][x];
    }
    return false;
}

void VirtualConsole::set(ui32 x, ui32 y, const Char& c) {
    if (x >= cols() || y >= rows()) {
        std::stringstream ss;
        ss << "Invalid character position, maximum = (" << cols() - 1 << ", " << rows() - 1 << "), got (" << x << ", "
           << y << ")";
        throw Exception(ss.str());
    }
    if (c == data_[y][x]) {
        return;
    }

    data_[y][x] = c;
    mask_[y][x] = true;
}

ui32 VirtualConsole::cursorX() const {
    return cursorX_;
}

ui32 VirtualConsole::cursorY() const {
    return cursorY_;
}

void VirtualConsole::setCursorPosition(ui32 x, ui32 y) {
    assert(x <= cols());
    assert(y <= rows());

    cursorX_ = x;
    cursorY_ = y;
}

void VirtualConsole::addChar(ui32 c) {
    if (cols() * rows() <= 0) {
        return;
    }

    switch (c) {
    case '\n':
    case '\r':
        ++cursorY_;
        cursorX_ = 0;
        break;
    case '\b':
        if (cursorX_ == 0) {
            cursorX_ = cols() - 1;
            cursorY_ = cursorY_ == 0 ? rows() - 1 : cursorY_ - 1;
        } else {
            --cursorX_;
        }
        set(cursorX_, cursorY_, Char(' ', data_[cursorY_][cursorX_].bg(), data_[cursorY_][cursorX_].fg()));
        break;
    case '\t':
        for (int i = 0; i < 4; ++i) {
            addChar(' ');
        }
        break;
    default:
        set(cursorX_, cursorY_, Char(c, data_[cursorY_][cursorX_].bg(), data_[cursorY_][cursorX_].fg()));
        ++cursorX_;
        if (cursorX_ >= cols()) {
            cursorX_ = 0;
            ++cursorY_;
        }
        if (cursorY_ >= rows()) {
            cursorY_ = 0;
        }
    }
    cursorX_ = (cursorX_ + 2 * cols()) % cols();
    cursorY_ = (cursorY_ + 2 * rows()) % rows();
}

std::vector<std::pair<ui32, ui32>> VirtualConsole::getUpdatedChars(bool force) {
    std::vector<std::pair<ui32, ui32>> result;
    for (ui32 j = 0; j < rows(); ++j) {
        for (ui32 i = 0; i < cols(); ++i) {
            if (mask_[j][i] || force) {
                mask_[j][i] = false;
                result.push_back(std::make_pair(i, j));
            }
        }
    }
    return result;
}

bool Char::operator==(const Char& other) const {
    return this->c() == other.c() && this->bg() == other.bg() && this->fg() == other.fg();
}

bool Char::operator!=(const Char& other) const {
    return !(*this == other);
}

} // namespace rterm
