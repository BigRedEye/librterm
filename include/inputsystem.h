#ifndef RTERM_INPUTSYSTEM_H
#define RTERM_INPUTSYSTEM_H

#include "key.h"
#include "keycode.h"

#include <functional>


namespace rterm {

class InputSystem {
public:
    InputSystem();

    Key getKey(int32_t timeout, std::function<bool(void)> isRunning) const;
    char_t getChar(int32_t timeout, std::function<bool(void)> isRunning) const;
    bool isKeyPressed(int code) const;
    void getMousePosition(int &x, int &y) const;
    uint8_t getMouseButtons() const;

private:
    const uint8_t *keyboardState_;
};

}

#endif // RTERM_INPUTSYSTEM_H
