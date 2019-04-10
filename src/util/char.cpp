#include "rterm/util/char.h"

#include <algorithm>
#include <codecvt>
#include <locale>
#include <memory>
#include <vector>
#include <cstdint>

namespace rterm {

/* Workaround for MSVC 15 */
#if defined(_MSC_VER) && _MSC_VER >= 1900
std::u32string utf8ToUtf32(const std::string& str) {
    static std::wstring_convert<std::codecvt_utf8<int32_t>, int32_t> convert;
    auto p = convert.from_bytes(str);
    std::u32string result(p.size(), 0);
    std::copy(p.data(), p.data() + p.size(), &result[0]);
    return result;
}

std::string utf32ToUtf8(const std::u32string& str) {
    std::vector<int32_t> data(str.size());
    /* We use C++14 */
    std::copy(str.data(), str.data() + str.size(), &data[0]);
    static std::wstring_convert<std::codecvt_utf8<int32_t>, int32_t> convert;
    return convert.to_bytes(data.data());
}
#else
std::u32string utf8ToUtf32(const std::string& str) {
    static std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
    return convert.from_bytes(str);
}

std::string utf32ToUtf8(const std::u32string& str) {
    static std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
    return convert.to_bytes(str);
}
#endif


} // namespace rterm
