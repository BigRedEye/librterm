#pragma once

#include "char.h"

#include <iostream>

namespace rterm {

/**
 * @brief The Logger class simplifies logging
 */
class Logger {
public:
    /**
     * @brief Enum of log messages priority
     */
    enum MessagePriority { LOG, INFO, WARNING, ERROR, CRITICAL };

    /**
     * @brief Constructs logger with given priority
     * @param prior priority of messages
     */
    Logger(MessagePriority prior = INFO);

    /**
     * @brief Destroys logger and flushes output if flush_ is true
     */
    ~Logger();

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    /**
     * @brief Set delimer character
     * @param delim delimer
     * @return reference to Logger object
     */
    Logger& setDelimer(char delim);

    /**
     * @brief Set flush flag
     * @param flush if true, output of logger will be flushed when ~Logger()
     * will be called
     * @return reference to Logger object
     */
    Logger& setFlush(bool flush);

    /**
     * @brief iostream-like output
     * @return reference to Logger object
     */
    template<typename T>
    inline Logger& operator<<(const T& msg) {
        std::cout << msg;
        if (delim_) {
            std::cout << delim_;
        }
        return *this;
    }

    /**
     * @brief cstdio-like output printf
     * @param fmt format string
     * @return reference to Logger object
     */
    template<typename... Args>
    Logger& printf(const std::string& fmt, Args&&... args) {
        return operator<<(format(fmt, std::forward<Args>(args)...));
    }

private:
    MessagePriority prior_;
    char delim_;
    bool flush_;
};

} // namespace rterm
