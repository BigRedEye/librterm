#ifndef RTERM_EVENT_H
#define RTERM_EVENT_H

#include "SDL2/SDL_events.h"

namespace rterm {

struct EventType {
    enum {
        Quit = 0,   /**< User-requested quit */
        Window,     /**< Window state change */
        System,     /**< System specific event */
        KeyDown,    /**< Key pressed */
        KeyUp,      /**< Key released */
        MouseMove,  /**< Mouse moved */
        MouseDown,  /**< Mouse button pressed */
        MouseUp,    /**< Mouse button released */
        MouseWheel, /**< Mouse wheel motion */
        Unknown,    /**< Unknown event */
        COUNT       /**< Size of enum */
    };
};

inline int eventTypeFromSDLEvent(SDL_EventType type);

class Event {
public:
    explicit Event(SDL_Event *event = nullptr);

    int type() const;

private:
    int type_;
};

class QuitEvent : public Event {
public:
    explicit QuitEvent(SDL_Event *event = nullptr);
};

class WindowEvent : public Event {
public:
    explicit WindowEvent(SDL_Event *event = nullptr);

    int x() const;
    int y() const;
    int windowEventType() const;
};

class SystemEvent : public Event {
public:
    explicit SystemEvent(SDL_Event *event = nullptr);
};
}

#endif // RTERM_EVENT_H
