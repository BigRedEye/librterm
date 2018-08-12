/**
 * @file
 * @brief This file contains key codes and key modifiers constants
 */

#pragma once


#include <SDL2/SDL_keycode.h>


namespace rterm {

/**
 * @brief Key codes enumeration. For alphanumeric keys their codes are the same as their char representation.
 */
enum KeyCode {
    UNKNOWN = SDLK_UNKNOWN, ///< Not recognized

    TAB = '\t', ///< Tab
    ENTER = '\r', ///< Enter
    BACKSPACE = '\b', ///< Backspace

    ESCAPE = SDLK_ESCAPE, ///< Escape
    CAPSLOCK = SDLK_CAPSLOCK, ///< CapsLock
    PRINTSCREEN = SDLK_PRINTSCREEN, ///< PrintScreen
    SCROLLLOCK = SDLK_SCROLLLOCK, ///< ScrollLock
    PAUSE = SDLK_PAUSE, ///< Pause
    INSERT = SDLK_INSERT, ///< Insert
    HOME = SDLK_HOME, ///< Home
    END = SDLK_END, ///< End
    PAGEUP = SDLK_PAGEUP, ///< PageUp
    PAGEDOWN = SDLK_PAGEDOWN, ///< PageDown
    DELETE = SDLK_DELETE, ///< Delete
    RIGHT = SDLK_RIGHT, ///< Right arrow
    LEFT = SDLK_LEFT, ///< Left arrow
    DOWN = SDLK_DOWN, ///< Down arrow
    UP = SDLK_UP, ///< Up arrow
    NUMLOCK = SDLK_NUMLOCKCLEAR, ///< NumLock

    F1  = SDLK_F1, ///< F1
    F2  = SDLK_F2, ///< F2
    F3  = SDLK_F3, ///< F3
    F4  = SDLK_F4, ///< F4
    F5  = SDLK_F5, ///< F5
    F6  = SDLK_F6, ///< F6
    F7  = SDLK_F7, ///< F7
    F8  = SDLK_F8, ///< F8
    F9  = SDLK_F9, ///< F9
    F10 = SDLK_F10, ///< F10
    F11 = SDLK_F11, ///< F11
    F12 = SDLK_F12, ///< F12
    F13 = SDLK_F13, ///< F13
    F14 = SDLK_F14, ///< F14
    F15 = SDLK_F15, ///< F15
    F16 = SDLK_F16, ///< F16
    F17 = SDLK_F17, ///< F17
    F18 = SDLK_F18, ///< F18
    F19 = SDLK_F19, ///< F19
    F20 = SDLK_F20, ///< F20
    F21 = SDLK_F21, ///< F21
    F22 = SDLK_F22, ///< F22
    F23 = SDLK_F23, ///< F23
    F24 = SDLK_F24, ///< F24

    /* numpad keys */
    NP_SLASH = SDLK_KP_DIVIDE, ///< Slash on the numpad (/)
    NP_ASTERISK = SDLK_KP_MULTIPLY, ///< Asterisk on the numpad (*)
    NP_MINUS = SDLK_KP_MINUS, ///< Minus on the numpad (-)
    NP_PLUS = SDLK_KP_PLUS, ///< Plus on the numpad (+)
    NP_ENTER = SDLK_KP_ENTER, ///< Enter on the numpad

    NP_1 = SDLK_KP_1, ///< Numpad 1
    NP_2 = SDLK_KP_2, ///< Numpad 2
    NP_3 = SDLK_KP_3, ///< Numpad 3
    NP_4 = SDLK_KP_4, ///< Numpad 4
    NP_5 = SDLK_KP_5, ///< Numpad 5
    NP_6 = SDLK_KP_6, ///< Numpad 6
    NP_7 = SDLK_KP_7, ///< Numpad 7
    NP_8 = SDLK_KP_8, ///< Numpad 8
    NP_9 = SDLK_KP_9, ///< Numpad 9
    NP_0 = SDLK_KP_0, ///< Numpad 0
    NP_DOT = SDLK_KP_PERIOD ///< Dot on the numpad
};

/**
 * @brief Key modifiers. Real modifiers state is an OR'ed combination of this constants
 */
enum KeyMod {
    NONE  = 0, ///< No modifier
    SHIFT = 1 << 0, ///< Shift
    CTRL  = 1 << 1, ///< Ctrl
    ALT   = 1 << 2, ///< Alt
    WIN   = 1 << 3, ///< Win
    NUM   = 1 << 4, ///< NumLock
    CAPS  = 1 << 5, ///< CapsLock
};

} // namespace rterm
