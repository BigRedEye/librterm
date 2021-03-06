#include "rterm/window/key.h"

namespace rterm {

Key::Key(SDL_Keycode code, ch32 ch)
    : mod_(0)
    , unicode_(ch) {
    setKey(code);
}

Key::Key(KeyCode code, ch32 ch)
    : Key(static_cast<int>(code), ch) {
}

Key::Key(ch32 ch)
    : code_(0)
    , mod_(0)
    , unicode_(ch) {
}

Key::Key(SDL_Keysym sym, ch32 ch)
    : Key(sym.sym, ch) {
    addMod(sym.mod);
}

ch32 Key::toChar() const {
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
    case rterm::KeyCode::NP_ENTER:
        unicode_ = '\r';
        break;
    default:
        break;
    }
    return *this;
}

Key& Key::setChar(ch32 c) {
    unicode_ = c;
    return *this;
}

Key& Key::addMod(ui32 mod) {
    ui32 mask = 1;
    for (int i = 0; i < 32; ++i, mask <<= 1) {
        if (mod & mask) {
            switch (mask) {
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
        }
    }
    return *this;
}

} // namespace rterm
