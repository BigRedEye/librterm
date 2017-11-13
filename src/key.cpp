#include "key.h"

namespace term {
Key::Key(SDL_Keycode code)
    : code_(code),
      mod_(0),
      unicode_(0) {
}

Key::Key(KeyCode code)
    : Key(reinterpret_cast<int&>(code)) {
}

Key::Key(char_t ch)
    : code_(0),
      mod_(0),
      unicode_(ch) {
}

char_t Key::toChar() const {
    return unicode_;
}

int Key::mod() const {
    return mod_;
}

int Key::key() const {
    return code_;
}

Key& Key::setKey(int code) {
    code_ = code;
    switch (code) {
    case '\n':
    case '\r':
    case '\b':
    case '\t':
        unicode_ = code;
        break;
    default:
        break;
    }
    return *this;
}

Key& Key::setMod(int mod) {
    mod_ = mod;
    return *this;
}

Key& Key::setChar(char_t c) {
    unicode_ = c;
    return *this;
}

Key& Key::addMod(SDL_Keymod mod) {
    switch (mod) {
    case KMOD_LSHIFT:
    case KMOD_RSHIFT:
        mod_ |= KeyMod::SHIFT;
        break;
    case KMOD_LCTRL:
    case KMOD_RCTRL:
        mod_ |= KeyMod::CTRL;
        break;
    case KMOD_LALT:
    case KMOD_RALT:
        mod_ |= KeyMod::ALT;
        break;
    case KMOD_LGUI:
    case KMOD_RGUI:
        mod_ |= KeyMod::WIN;
        break;
    case KMOD_NUM:
        mod_ |= KeyMod::NUM;
        break;
    default:
        break;
    }
    return *this;
}
}
