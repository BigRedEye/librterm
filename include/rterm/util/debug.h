#pragma once

#include <csignal>

#include <rterm/util/exception.h>

#if RTERM_DEBUG
#if defined(RTERM_OS_UNIX)
#define RTERM_BREAK std::raise(SIGINT)
#elif defined(RTERM_OS_WINDOWS)
#define RTERM_BREAK *reinterpret_cast<volatile int*>(nullptr) = 0
#else
#define RTERM_BREAK
#endif // RTERM_OS
#else  // RTERM_DEBUG
#define RTERM_BREAK
#endif // RTERM_DEBUG

#define RTERM_THROW(...)                \
    throw SourceException {             \
        __LINE__, __FILE__, __VA_ARGS__ \
    }

#if RTERM_DEBUG
#define RTERM_ASSERT(stmt, ...)                        \
    if (!(stmt)) {                                     \
        RTERM_THROW("Assertion failed: " __VA_ARGS__); \
    }
#else
#define RTERM_ASSERT(...)
#endif

#define RTERM_ENSURE(stmt, ...)                     \
    if (!(stmt)) {                                  \
        RTERM_THROW("Ensure failed: " __VA_ARGS__); \
    }
