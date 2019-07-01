#pragma once

#include <rterm/unicode/grapheme.h>

#include <vector>
#include <string>

namespace rterm {

class GraphemeString {
public:
    GraphemeString(std::string_view utf8);

    Grapheme operator[](size_t index) const;
    void replace(size_t index, Grapheme grapheme);

private:
    bool changed_{true};
    std::string utf8_;
    std::vector<size_t> bounds_;
};

} // namespace rterm 
