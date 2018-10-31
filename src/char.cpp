#include "char.h"

#include <codecvt>
#include <locale>
#include <memory>

namespace rterm {

std::u32string utf8ToUtf32(const std::string& str) {
    static std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
    return convert.from_bytes(str);
}

std::string utf32ToUtf8(const std::u32string& str) {
    static std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
    return convert.to_bytes(str);
}

} // namespace rterm
