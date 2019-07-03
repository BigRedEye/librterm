#pragma once

#ifdef __has_include
#if (__has_include(<filesystem>))

#include <filesystem>
namespace rterm {
namespace fs = std::filesystem;
} // namespace rterm

#elif (__has_include(<experimental/filesystem>))

#include <experimental/filesystem>
namespace rterm {
namespace fs = std::experimental::filesystem;
} // namespace rterm

#else // __has_include

#include <filesystem>
namespace rterm {
namespace fs = std::filesystem;
} // namespace rterm

#endif // __has_include

