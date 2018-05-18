/**
 * @file
 * @brief This file contains some helper functions
 * for working with UTF8 characters
 */

#pragma once

#include <string>
#include <assert.h>
#include <cstdarg>

/**
 * @brief Namespace for all rterm functionality
 * @see https://github.com/BigRedEye/rterm
 */
namespace rterm {
typedef char32_t char_t;

/**
 * @brief Convert a number of UTF8 bytes to UTF32 character
 * @param[in] str std::string that contains an UTF8 characte represented by bytes
 * @return Converted character
 * @note This function shouldn't be used to convert multiple UTF8 characters
 * @see rterm::UTF32ToBytes
 */
char_t BytesToUTF32(const std::string& str);

/**
 * @brief Convert an UTF8 character to a number of bytes
 * @param[in] c UTF8 character
 * @return std::string that contains an input character represented by bytes
 * @see rterm::UTF8BytesToChar
 */
std::string UTF32ToBytes(char_t c);

/**
 * @brief Get printf-like formatted string
 * @param fmt printf-style format string
 * @return formatted string
 */
std::string format(const std::string& fmt, ...);

/**
 * @brief Get printf-like formatted string
 * @param fmt printf-style format string
 * @param args va_list of arguments
 * @return formatted string
 */
std::string vformat(const std::string& fmt, va_list args);
}
