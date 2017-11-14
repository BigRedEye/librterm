#ifndef KEY_H
#define KEY_H

#include <SDL2/SDL_keycode.h>
#include "char.h"

namespace term {

enum KeyCode {
    UNKNOWN = 0,

    TAB = '\t',
    ENTER = '\r',
    BACKSPACE = '\b',

    ESCAPE = SDLK_ESCAPE,
    CAPSLOCK = SDLK_CAPSLOCK,
    PRINTSCREEN = SDLK_PRINTSCREEN,
    SCROLLLOCK = SDLK_SCROLLLOCK,
    PAUSE = SDLK_PAUSE,
    INSERT = SDLK_INSERT,
    HOME = SDLK_HOME,
    END = SDLK_END,
    PAGEUP = SDLK_PAGEUP,
    PAGEDOWN = SDLK_PAGEDOWN,
    DELETE = SDLK_DELETE,
    RIGHT = SDLK_RIGHT,
    LEFT = SDLK_LEFT,
    DOWN = SDLK_DOWN,
    UP = SDLK_UP,
    NUMLOCK = SDLK_NUMLOCKCLEAR,

    F1  = SDLK_F1,
    F2  = SDLK_F2,
    F3  = SDLK_F3,
    F4  = SDLK_F4,
    F5  = SDLK_F5,
    F6  = SDLK_F6,
    F7  = SDLK_F7,
    F8  = SDLK_F8,
    F9  = SDLK_F9,
    F10 = SDLK_F10,
    F11 = SDLK_F11,
    F12 = SDLK_F12,
    F13 = SDLK_F13,
    F14 = SDLK_F14,
    F15 = SDLK_F15,
    F16 = SDLK_F16,
    F17 = SDLK_F17,
    F18 = SDLK_F18,
    F19 = SDLK_F19,
    F20 = SDLK_F20,
    F21 = SDLK_F21,
    F22 = SDLK_F22,
    F23 = SDLK_F23,
    F24 = SDLK_F24,

    /** numpad keys */
    NP_SLASH = SDLK_KP_DIVIDE,
    NP_ASTERISK = SDLK_KP_MULTIPLY,
    NP_MINUS = SDLK_KP_MINUS,
    NP_PLUS = SDLK_KP_PLUS,

    NP_1 = SDLK_KP_1,
    NP_2 = SDLK_KP_2,
    NP_3 = SDLK_KP_3,
    NP_4 = SDLK_KP_4,
    NP_5 = SDLK_KP_5,
    NP_6 = SDLK_KP_6,
    NP_7 = SDLK_KP_7,
    NP_8 = SDLK_KP_8,
    NP_9 = SDLK_KP_9,
    NP_0 = SDLK_KP_0,
    NP_DOT = SDLK_KP_PERIOD
};

enum KeyMod {
    NONE  = 0,
    SHIFT = 1 << 0,
    CTRL  = 1 << 1,
    ALT   = 1 << 2,
    WIN   = 1 << 3,
    NUM   = 1 << 4,
    CAPS  = 1 << 5,
};

class Key {
public:
    Key(SDL_Keycode code = SDLK_UNKNOWN);
    Key(KeyCode code);
    Key(char_t ch);

    char_t toChar() const;
    int key() const;
    int mod() const;
    
    Key& setKey(int k);
    Key& setChar(char_t c);

    friend class Term;
private:
    Key& addMod(Uint16 mod);

    SDL_Keycode code_;
    int mod_;
    char_t unicode_;
};
}

#endif // KEY_H
