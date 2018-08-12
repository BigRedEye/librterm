#pragma once


#include <stdexcept>
#include <string>


namespace rterm {

class Exception : public std::exception {
public:
    Exception();
    Exception(const std::string&);
    virtual ~Exception() noexcept = default;

    virtual const char* what() const noexcept;

private:
    std::string error_;
};

class BadTexture final : public Exception {
public:
    BadTexture();
};

class BadRenderer final : public Exception {
public:
    BadRenderer();
};

class BadFont final : public Exception {
public:
    BadFont();
};

} // namespace rterm
