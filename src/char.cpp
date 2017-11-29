#include "char.h"

#include <codecvt>
#include <locale>
#include <memory>
#include <cstdarg>

namespace rterm {

char_t UTF8BytesToChar(const std::string &str) {
    static std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
    return static_cast<char_t>(convert.from_bytes(str)[0]);
}

std::string UTF8CharToBytes(char_t c) {
    static std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
    return convert.to_bytes(c);
}

std::string format(const std::string &fmt) {
    return fmt;
}

std::string format(const std::string &fmt, ...) {
    va_list args;
    va_start(args, fmt);
    auto str = vformat(fmt, args);
    va_end(args);
    return str;
}

std::string vformat(const std::string &fmt, va_list args) {
    va_list argsCopy;
    va_copy(argsCopy, args);

    int bufSize = vsnprintf(nullptr, 0, fmt.c_str(), args);
    if (bufSize < 0) {
        va_end(argsCopy);
        return "";
    }
    std::unique_ptr<char[]> buf(new char[bufSize + 1]);
    vsnprintf(buf.get(), bufSize + 1, fmt.c_str(), argsCopy);
    va_end(argsCopy);
    return std::string(buf.get());
}
}
