#pragma once

#include <stdexcept>
#include <string>

#include <fmt/format.h>

namespace rterm {

template<typename Fmt, typename... Args>
inline std::string nothrow_fmt(Fmt&& format, Args&&... args) noexcept {
    try {
        return fmt::format(std::forward<Fmt>(format), std::forward<Args>(args)...);
    } catch (const fmt::format_error& err) {
        try {
            return fmt::format("Caught format exception while forming exception message: {}", err.what());
        } catch (const fmt::format_error&) {
            return "Caught format exception while forming exception message";
        }
    }
}

class Exception : public std::exception {
public:
    Exception(const std::string& msg);

    template<typename Fmt, typename Arg, typename... Args>
    Exception(Fmt&& format, Arg&& arg, Args&&... args)
        : Exception(nothrow_fmt(std::forward<Fmt>(format), std::forward<Arg>(arg), std::forward<Args>(args)...)) {
    }

    virtual ~Exception() noexcept;

    virtual const char* what() const noexcept;

private:
    std::string msg_;
};

class SourceException final : public Exception {
public:
    template<typename Fmt, typename... Args>
    SourceException(size_t line, const char* path, Fmt&& format, Args&&... args)
        : Exception{std::string{"[file: {}, line: {}] "} + std::forward<Fmt>(format),
                    path,
                    line,
                    std::forward<Args>(args)...} {
    }
};

} // namespace rterm
