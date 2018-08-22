/**
 * @file
 * @brief This file contains key representation
 */

#pragma once

#include "char.h"
#include "keycode.h"

#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>

namespace rterm {

/**
 * @brief Pressed key representation
 *
 * This class represents pressed key and its modifiers
 */
class Key {
public:
    /**
     * @brief Construct Key from SDL_Keycode
     * @param code SDL_Keycode constant
     */
    explicit Key(SDL_Keycode code = SDLK_UNKNOWN, char_t ch = 0);

    /**
     * @brief Construct Key from KeyCode
     * @param code KeyCode constant
     * @see keycode.h for the list of key codes
     */
    explicit Key(KeyCode code, char_t ch = 0);

    /**
     * @brief Construct Key from unicode character
     * @param ch unicode char
     */
    explicit Key(char_t ch);

    /**
     * @brief Construct Key from SDL_Keysym
     * @param sym SDL_Keysym from SDL events
     */
    explicit Key(SDL_Keysym sym, char_t ch = 0);

    inline operator bool() const {
        return code_;
    }

    /**
     * @brief Get printable unicode char of the Key
     * @return printable unicode char
     * @return 0, if this key doesn't have printable representation,for example,
     * rterm::KeyCode::LEFT
     */
    char_t toChar() const;

    /**
     * @brief Gey KeyCode of a key if any
     * @return KeyCode constant
     * @see keycode.h for the list of key codes
     */
    int key() const;

    /**
     * @brief Get modifiers of pressed key
     * @return OR'ed KeyMod constants
     * @see keycode.h for the list of key modifiers
     */
    int mod() const;

private:
    /// @cond INTERNAL

    /**
     * @brief Set KeyCode of current Key
     * @param code KeyCode constant
     * @return reference to this object
     * @see keycode.h for the list of key codes
     */
    Key& setKey(int code);

    /**
     * @brief Set unicode char of current Key
     * @param c unicode char
     * @return reference to this object
     */
    Key& setChar(char_t c);

    /**
     * @brief add modifier to this key
     * @param mod OR'ed KeyMod constants
     * @return reference to this object
     * @see keycode.h for the list of key modifiers
     */
    Key& addMod(ui32 mod);

private:
    SDL_Keycode code_; ///< real code value
    ui32 mod_;         ///< OR'ed KeyMod constants
    char_t unicode_;   ///< unicode char

    /// @endcond
};

} // namespace rterm
