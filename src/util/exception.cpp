#include <rterm/util/exception.h>

namespace rterm {

Exception::Exception(const std::string& msg)
    : msg_{msg} {
}

Exception::~Exception() noexcept {
}

const char* Exception::what() const noexcept {
    return msg_.data();
}

} // namespace rterm
