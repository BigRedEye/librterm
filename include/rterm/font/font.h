#pragma once

#include <rterm/util/defs.h>
#include <rterm/util/named_args.h>

namespace rterm::args {

RTERM_DECLARE_NAMED_ARGUMENT(fs::path, path);
RTERM_DECLARE_NAMED_ARGUMENT(i32, priority);

}

namespace rterm::font {

class Font {
public:
    template<typename ...Args>
    Font(Args&& ...args) {
        ((fillArgs(std::forward<Args>(args)), 0) + ... + 0);
    }

    const fs::path& path() const;
    i32 priority() const;

private:
    void fillArgs(args::pathTag);
    void fillArgs(args::priorityTag);
    template<typename T, bool False = false>
    void fillArgs(T) {
        static_assert(False, "Invalid argument");
    }

private:
    fs::path path_;
    i32 priority_{0};
};

} // namespace rterm
