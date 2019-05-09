#pragma once

#include <rterm/util/api.h>
#include <rterm/util/defs.h>
#include <rterm/util/named_args.h>
#include <rterm/util/vector.h>

namespace rterm::terminal {

RTERM_DECLARE_NAMED_ARGUMENT(u32, width);
RTERM_DECLARE_NAMED_ARGUMENT(u32, height);
RTERM_DECLARE_NAMED_ARGUMENT(f32, opacity);
RTERM_DECLARE_NAMED_ARGUMENT(uSize, size);
RTERM_DECLARE_NAMED_ARGUMENT(bool, resizeable);
RTERM_DECLARE_NAMED_ARGUMENT(bool, fullscreen);
RTERM_DECLARE_NAMED_ARGUMENT(Api, api);
RTERM_DECLARE_NAMED_ARGUMENT(std::string, title);

class Options {
public:
    template<typename... Args>
    Options(Args&&... args) {
        ((fillOptions(std::forward<Args>(args)), 0) + ... + 0);
    }

    friend class Terminal;

private:
    void fillOptions(widthTag tag);
    void fillOptions(heightTag tag);
    void fillOptions(opacityTag tag);
    void fillOptions(resizeableTag tag);
    void fillOptions(fullscreenTag tag);
    void fillOptions(apiTag tag);
    void fillOptions(titleTag tag);

    template<typename T, bool False = false>
    void fillOptions(T&&) {
        static_assert(False, "Invalid argument");
    }

private:
    uSize size_{800, 600};
    f32 opacity_{1.0};
    bool fullscreen_{false};
    bool resizeable_{true};
    Api apis_{Api::best};
    std::string title_{};
};

} // namespace rterm::terminal
