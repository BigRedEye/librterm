#pragma once

#include <stdexcept>
#include <string>

namespace rterm {

class Exception : public std::exception {
public:
    Exception(const std::string& msg);
    virtual ~Exception() noexcept;

    virtual const char* what() const noexcept;

private:
    std::string msg_;
};

} // namespace rterm
