#include "key.h"


namespace rterm {

Key::Key(SDL_Keycode code, char_t ch)
    : mod_(0)
    , unicode_(ch)
{
    setKey(code);
}

Key::Key(KeyCode code, char_t ch)
    : Key(reinterpret_cast<int&>(code), ch)
{
}

Key::Key(char_t ch)
    : code_(0)
    , mod_(0)
    , unicode_(ch)
{
}

Key::Key(SDL_Keysym sym, char_t ch)
    : Key(sym.sym, ch)
{
    addMod(sym.mod);
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
    case rterm::KeyCode::NP_ENTER:
        unicode_ = '\r';
        break;
    default:
        break;
    }
    return *this;
}

Key& Key::setChar(char_t c) {
    unicode_ = c;
    return *this;
}

Key& Key::addMod(Uint16 mod) {
    Uint16 mask = 1;
    for (int i = 0; i < 16; ++i, mask <<= 1) {
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
