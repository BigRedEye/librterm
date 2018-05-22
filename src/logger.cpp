#include "char.h"
#include "logger.h"

#include <chrono>
#include <cstdarg>
#include <iomanip>
#include <iostream>
#include <string>


namespace rterm {

Logger::Logger(Logger::MessagePriority priority)
    : prior_(priority)
    , delim_(0)
    , flush_(true)
{
    *this << "[ ";

    switch (priority) {
    case Logger::LOG:
        *this << "LOG";
        break;
    case Logger::INFO:
        *this << "INFO";
        break;
    case Logger::WARNING:
        *this << "WARNING";
        break;
    case Logger::ERROR:
        *this << "ERROR";
        break;
    case Logger::CRITICAL:
        *this << "CRITICAL";
        break;
    default:
        break;
    }
    *this << " ] ";
    std::time_t t = std::time(nullptr);
    *this << std::put_time(std::gmtime(&t), "%H:%M:%S") << ' ';
}

Logger::~Logger() {
    if (flush_ || prior_ >= ERROR) {
        std::cout << std::endl;
    } else {
        *this << '\n';
    }
}

Logger& Logger::setDelimer(char delim) {
    delim_ = delim;
    return *this;
}

Logger& Logger::setFlush(bool flush) {
    flush_ = flush;
    return *this;
}

Logger& Logger::printf(const std::string& fmt, ...) {
    va_list args;
    va_start(args, fmt);
    auto str = vformat(fmt, args);
    va_end(args);
    operator<<(str);
    return *this;
}

} // namespace rterm
