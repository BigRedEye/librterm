#ifndef KEY_H
#define KEY_H

#include <SDL2/SDL_keycode.h>

namespace term {
class Key
{
public:
    Key();
    char toChar() const;

    enum class KeyCode {

    };
private:
    int _keycode;
};
}

#endif // KEY_H
