/**
 * @file
 * @brief This file contains mouse buttons constants
 */

#pragma once

#include <SDL2/SDL_mouse.h>

namespace rterm {

/**
 * @brief mouse buttons masks
 */
struct MouseButton {
    enum Button {
        LEFT = SDL_BUTTON_LMASK,
        MID = SDL_BUTTON_MMASK,
        RIGHT = SDL_BUTTON_RMASK,
        X1 = SDL_BUTTON_X1MASK,
        X2 = SDL_BUTTON_X2MASK,
    };
};

} // namespace rterm
