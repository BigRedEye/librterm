#ifndef EVENT_H
#define EVENT_H

#include <SDL2/SDL_events.h>

namespace term {
class Event {
public:
    Event(SDL_Event *ev);

    enum class EventType {
        Quit            = 0x100,
        /* Window events */
        WindowEvent     = 0x200, /**< Window state change */
        SysWMEevent,             /**< System specific event */
        /* Keyboard events */
        KeyDown         = 0x300, /**< Key pressed */
        KeyUp,                   /**< Key released */
        TextEditing,             /**< Keyboard text editing (composition) */
        TextInput,               /**< Keyboard text input */
        KeyMapChanged,           /**< Keymap changed due to a system event such as an
                                         input language or keyboard layout change. */
        /* Mouse events */
        MouseMotion     = 0x400, /**< Mouse moved */
        MouseButtonDown,         /**< Mouse button pressed */
        MouseButtonUp,           /**< Mouse button released */
        MouseWheel,              /**< Mouse wheel motion */
        /* Clipboard events */
        ClipboardUpdate = 0x900, /**< The clipboard changed */
    };
};
}

#endif // EVENT_H
