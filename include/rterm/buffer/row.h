#pragma once

#include <rterm/cell/cell.h>
#include <rterm/util/defs.h>
#include <rterm/util/vector.h>
#include <vector>

namespace rterm {

class Row {
public:
    const Cell& get(size_t pos) const;
    void set(size_t pos, const Cell& cell);
    void add(std::string_view utf8);

private:
    std::vector<Cell> cells_;
    std::vector<size_t> bounds_;
    std::string utf8_;
};

} // namespace rterm
