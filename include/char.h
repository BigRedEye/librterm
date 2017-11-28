/**
 * @file
 * @brief This file contains some helper functions 
 * for working with UTF8 characters
 * @author BigRedEye
 */

#ifndef RTERM_CHAR_H
#define RTERM_CHAR_H

#include <codecvt>
#include <locale>
#include <string>

#include "color.h"

/**
 * @brief Namespace for all rterm functionality
 * @see https://github.com/BigRedEye/rterm
 */
namespace rterm {
typedef char32_t char_t;

/**
 * @brief Convert a number of UTF8 bytes to single character
 * @param[in] str std::string that contains an UTF8 characte represented by bytes
 * @return Converted character
 * @note This function shouldn't be used to convert multiple UTF8 characters
 * @see rterm::UTF8CharToBytes
 */
inline char_t UTF8BytesToChar(const std::string &str) {
    static std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
    return static_cast<char_t>(convert.from_bytes(str)[0]);
}

/**
 * @brief Convert an UTF8 character to a number of bytes
 * @param[in] c UTF8 character
 * @return std::string that contains an input character represented by bytes
 * @see rterm::UTF8BytesToChar
 */
inline std::string UTF8CharToBytes(char_t c) {
    static std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
    return convert.to_bytes(c);
}
}

#endif // RTERM_CHAR_H
