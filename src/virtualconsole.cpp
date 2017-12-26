#include "virtualconsole.h"

namespace rterm {
VirtualConsole::VirtualConsole()
    : VirtualConsole(0, 0) {
}

VirtualConsole::VirtualConsole(size_t ncols, size_t nrows)
    : cursorX_(0),
      cursorY_(0) {
    data_.assign(nrows, std::vector<Char>(ncols, ' '));
    mask_.assign(nrows, std::vector<char>(ncols, false));
}

size_t VirtualConsole::cols() const {
    if (data_.empty())
        return 0;
    return data_[0].size();
}

size_t VirtualConsole::rows() const {
    return data_.size();
}

void VirtualConsole::resize(size_t ncols, size_t nrows, Color bgColor, Color fgColor) {
    Char defaultChar(' ', bgColor, fgColor);
    data_.resize(nrows, std::vector<Char>(ncols, defaultChar));
    mask_.resize(nrows, std::vector<char>(ncols, true));
    for (auto& line : data_)
        line.resize(ncols, defaultChar);

    for (auto& line : mask_)
        line.resize(ncols, true);

    cursorX_ = std::min(cursorX_, ncols);
    cursorY_ = std::min(cursorY_, nrows);
}

Char VirtualConsole::get(size_t x, size_t y) const {
    if (x < cols() && y < rows())
        return data_[y][x];
    return Char(' ');
}

bool VirtualConsole::getMask(size_t x, size_t y) const {
    if (x < cols() && y < rows())
        return mask_[y][x];
    return false;
}

void VirtualConsole::set(size_t x, size_t y, Char c) {
    if (c == data_[y][x])
        return;

    data_[y][x] = c;
    mask_[y][x] = true;
}

size_t VirtualConsole::cursorX() const {
    return cursorX_;
}

size_t VirtualConsole::cursorY() const {
    return cursorY_;
}

void VirtualConsole::setCursorPosition(size_t x, size_t y) {
    assert(x <= cols());
    assert(y <= rows());

    cursorX_ = x;
    cursorY_ = y;
}

void VirtualConsole::addChar(char_t c) {
    if (cols() * rows() <= 0)
        return;

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
        }
        else
            --cursorX_;
        set(cursorX_, cursorY_, Char(' ',
                                     data_[cursorY_][cursorX_].bg(),
                                     data_[cursorY_][cursorX_].fg()));
        break;
    case '\t':
        for (int i = 0; i < 4; ++i)
            addChar(' ');
        break;
    default:
        set(cursorX_, cursorY_, Char(c,
                                     data_[cursorY_][cursorX_].bg(),
                                     data_[cursorY_][cursorX_].fg()));
        ++cursorX_;
        if (cursorX_ >= cols())
            cursorX_ = 0, ++cursorY_;
        if (cursorY_ >= rows())
            cursorY_ = 0;
    }
    cursorX_ = (cursorX_ + 2 * cols()) % cols();
    cursorY_ = (cursorY_ + 2 * rows()) % rows();
}

void VirtualConsole::shift(int dx, int dy) {
    std::vector<std::vector<Char>> tmpData(rows(), std::vector<Char>(cols()));
    std::vector<std::vector<char>> tmpMask(rows(), std::vector<char>(cols()));

    for (size_t j = 0; j < rows(); ++j)
        for (size_t i = 0; i < cols(); ++i)
            if (j + dy >= 0 && i + dx >= 0) {
                tmpData[j][i] = get(i + dx, j + dy);
                tmpMask[j][i] = getMask(i + dx, j + dy);
            }
    data_.swap(tmpData);
    mask_.swap(tmpMask);
}

std::vector<std::pair<size_t, size_t>> VirtualConsole::getUpdatedChars(bool force) {
    std::vector<std::pair<size_t, size_t>> result;
    for (size_t j = 0; j < rows(); ++j)
        for (size_t i = 0; i < cols(); ++i)
            if (mask_[j][i] || force) {
                mask_[j][i] = false;
                result.push_back(std::make_pair(i, j));
            }
    return result;
}

bool Char::operator ==(const Char &other) const {
    return this->c() == other.c() && 
           this->bg() == other.bg() && 
           this->fg() == other.fg();
}

bool Char::operator !=(const Char &other) const {
    return !(*this == other);
}

}
