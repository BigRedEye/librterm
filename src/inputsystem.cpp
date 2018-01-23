#include "inputsystem.h"
#include "char.h"

#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_timer.h>

namespace rterm {

InputSystem::InputSystem()
    : keyboardState_(SDL_GetKeyboardState(NULL)) {
}

Key InputSystem::getKey(int32_t timeout, std::function<bool(void)> isRunning) const {
    uint32_t endTick = SDL_GetTicks() + static_cast<uint32_t>(timeout);
    SDL_StartTextInput();
    while (isRunning() && (timeout == -1 || !SDL_TICKS_PASSED(SDL_GetTicks(), endTick))) {
        SDL_Event evs[2];
        SDL_PumpEvents();
        if (timeout < 0ll)
            SDL_WaitEvent(NULL);
        else {
            SDL_WaitEventTimeout(NULL, static_cast<uint32_t>(timeout));
            timeout = static_cast<int32_t>(endTick) - static_cast<int32_t>(SDL_GetTicks());
            if (timeout < 0)
                break;
        }
        int sz = 0;
        if (SDL_PeepEvents(&evs[sz], 1, SDL_GETEVENT, SDL_KEYDOWN, SDL_KEYDOWN) > 0)
            ++sz;
        if (SDL_PeepEvents(&evs[sz], 1, SDL_GETEVENT, SDL_TEXTINPUT, SDL_TEXTINPUT) > 0)
            ++sz;
        Key result;
        for (int i = 0; i < sz; ++i)
            switch (evs[i].type) {
            case SDL_KEYDOWN:
                result.setKey(evs[i].key.keysym.sym)
                      .addMod(evs[i].key.keysym.mod);
                break;
            case SDL_TEXTINPUT:
                result.setChar(UTF8BytesToChar(evs[i].text.text));
                break;
            default:
                break;
            }
        if (result.key() || result.toChar()) {
            return result;
        }
    }
    return Key();
}

char_t InputSystem::getChar(int32_t timeout, std::function<bool(void)> isRunning) const {
    Key key;
    uint32_t endTick = SDL_GetTicks() + static_cast<uint32_t>(timeout);
    while (isRunning()) {
         key = getKey(timeout, isRunning);
         if (key.toChar())
             break;
         if (timeout >= 0) {
             timeout = static_cast<int32_t>(endTick) - static_cast<int32_t>(SDL_GetTicks());
             if (timeout < 0)
                 break;
         }
    }
    return key.toChar();
}

bool InputSystem::isKeyPressed(int code) const {
    int scancode = SDL_GetScancodeFromKey(code);
    return keyboardState_[scancode];
}

void InputSystem::getMousePosition(int &x, int &y) const {
    SDL_GetMouseState(&x, &y);
}

uint8_t InputSystem::getMouseButtons() const {
    return SDL_GetMouseState(NULL, NULL);
}

}
