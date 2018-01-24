#ifndef RTERM_INPUTSYSTEM_H
#define RTERM_INPUTSYSTEM_H

#include "key.h"
#include "keycode.h"

#include <functional>

namespace rterm {

/// @cond INTERNAL

/**
 * @brief The InputSystem class handles user input
 */
class InputSystem {
public:
    InputSystem();

    /**
     * @brief wait for next key
     * @param timeout if not -1, this function returns Key(rterm::UNKNOWN) after timeout milliseconds
     * @param isRunning functor; after isRunning returns false, this function returns Key(UNKNOWN)
     * @return next key pressed by user
     * @see getChar if you want only printable characters
     */
    Key getKey(int32_t timeout, std::function<bool(void)> isRunning) const;

    /**
     * @brief wait for next key that has printable representation
     * @param timeout if not -1, this function returns Key(rterm::UNKNOWN) after timeout milliseconds
     * @param isRunning functor; after isRunning returns false, this function returns Key(UNKNOWN)
     * @return next key pressed by user
     * @see getKey if you want to handle all keys
     */
    char_t getChar(int32_t timeout, std::function<bool(void)> isRunning) const;

    /**
     * @brief Check if certain key is pressed
     * @param code KeyCode
     * @return is this key is pressed
     * @see keycode.h for the list of keycodes
     */
    bool isKeyPressed(int code) const;

    /**
     * @brief Get mouse position in pixels
     * @param[out] x coordinate
     * @param[out] y coordinate
     */
    void getMousePosition(int &x, int &y) const;

    /**
     * @brief Get mouse buttons state 
     * @return or'ed bitmask of mouse buttons
     * @see mouse.h for the list of mouse button masks
     */
    uint8_t getMouseButtons() const;

private:
    const uint8_t *keyboardState_; ///< An array that represents keyboard state. It is provided by SDL and shouldn't be deleted or updated.
};

/// @endcond

}

#endif // RTERM_INPUTSYSTEM_H
