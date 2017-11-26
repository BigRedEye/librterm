#include "virtualconsole.h"

namespace rterm {
VirtualConsole::VirtualConsole()
    : VirtualConsole(0, 0) {
}

VirtualConsole::VirtualConsole(size_t ncols, size_t nrows)
    : cursorX(0),
      cursorY(0) {
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

void VirtualConsole::resize(size_t ncols, size_t nrows) {
    data_.resize(nrows, std::vector<Char>(ncols, ' '));
    mask_.resize(nrows, std::vector<char>(ncols, true));
    for (auto& line : data_)
        line.resize(ncols, ' ');

    for (auto& line : mask_)
        line.resize(ncols, true);

    cursorX = std::min(cursorX, ncols);
    cursorY = std::min(cursorY, nrows);
}

Char VirtualConsole::get(size_t x, size_t y) const {
    return data_[y][x];
}

void VirtualConsole::set(size_t x, size_t y, Char c) {
    data_[y][x] = c;
    mask_[y][x] = true;
}

void VirtualConsole::addChar(Char c) {
    switch (c.c()) {
    case '\n':
    case '\r':
        ++cursorY;
        cursorX = 0;
        break;
    case '\b':
        --cursorX;
        if (cursorX < 0)
            cursorX = cols() - 1, --cursorY;
        if (cursorY < 0)
            cursorY = rows() - 1;
        data_[cursorY][cursorX].ch_ = ' ';
        mask_[cursorY][cursorX] = true;
        break;
    case '\t':
        for (int i = 0; i < 4; ++i)
            addChar(Char(' ', c.bg(), c.fg()));
        break;
    default:
        mask_[cursorY][cursorX] = true;
        data_[cursorY][cursorX] = c;
        ++cursorX;
        if (cursorX >= cols())
            cursorX = 0, ++cursorY;
        if (cursorY >= rows())
            cursorY = 0;
    }
    cursorX = (cursorX + 2 * cols()) % cols();
    cursorY = (cursorY + 2 * rows()) % rows();
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
}
