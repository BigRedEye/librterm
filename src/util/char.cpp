#include "rterm/util/char.h"

#include <utf8.h>
#include <utf8/cpp11.h>

namespace rterm {

std::u32string utf8ToUtf32(const std::string& str) {
    return utf8::utf8to32(str);
}

std::string utf32ToUtf8(const std::u32string& str) {
    return utf8::utf32to8(str);
}

} // namespace rterm
