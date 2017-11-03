#include "key.h"

namespace term {
Key::Key(SDL_Keycode code)
    : code_(code),
      mod_(KMOD_NONE){
}

template<typename T>
Key::Key(T code)
    : Key(reinterpret_cast<SDL_Keycode>(code)) {
}
}
