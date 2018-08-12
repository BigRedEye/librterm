/**
 * @file
 * @brief This file contains some helper functions
 * for working with UTF8 characters
 */

 
#pragma once


#include <assert.h>
#include <cstdarg>
#include <string>


/**
 * @brief Namespace for all rterm functionality
 * @see https://github.com/BigRedEye/rterm
 */
namespace rterm {
typedef char32_t char_t;

std::u32string Utf8ToUtf32(const std::string& str);

std::string Utf32ToUtf8(const std::u32string& str);

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

} // namespace rterm
