/**
 * @file
 * @brief This file contains some helper functions
 * for working with UTF8 characters
 */

#pragma once

#include <rterm/util/util.h>

#include <assert.h>
#include <cstdio>
#include <string>

/**
 * @brief Namespace for all rterm functionality
 * @see https://github.com/BigRedEye/rterm
 */
namespace rterm {

std::u32string utf8ToUtf32(const std::string& str);

std::string utf32ToUtf8(const std::u32string& str);

/**
 * @brief Get printf-like formatted string
 * @param fmt printf-style format string
 * @return formatted string
 */
template<typename... Args>
std::string format(const std::string& fmt, Args&&... args) {
    int bufSize = snprintf(nullptr, 0, fmt.data(), std::forward<Args>(args)...);
    if (bufSize < 0) {
        return "";
    }
    std::string result(bufSize + 1, '\0');
    snprintf(&result[0], result.size(), fmt.data(), std::forward<Args>(args)...);
    return result;
}

} // namespace rterm
