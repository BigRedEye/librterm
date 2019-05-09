#include <rterm/terminal/options.h>

namespace rterm::terminal {

void Options::fillOptions(widthTag tag) {
    size_[0] = tag;
}

void Options::fillOptions(heightTag tag) {
    size_[1] = tag;
}

void Options::fillOptions(opacityTag tag) {
    opacity_ = tag;
}

void Options::fillOptions(resizeableTag tag) {
    resizeable_ = tag;
}

void Options::fillOptions(fullscreenTag tag) {
    fullscreen_ = tag;
}

void Options::fillOptions(apiTag tag) {
    apis_ = tag;
}

void Options::fillOptions(titleTag tag) {
    title_ = tag;
}

} // namespace rterm::terminal
