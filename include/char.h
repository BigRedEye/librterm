#ifndef RTERM_CHAR_H
#define RTERM_CHAR_H

#include <codecvt>
#include <locale>
#include <string>

namespace term {
typedef char32_t char_t;
inline char_t UTF8BytesToChar(const std::string &str) {
    static std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
    return static_cast<char_t>(convert.from_bytes(str)[0]);
}
inline std::string UTF8CharToBytes(char_t c) {
    static std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
    return convert.to_bytes(c);

}
}

#endif // RTERM_CHAR_H
