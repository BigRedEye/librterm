#pragma once


#include "key.h"
#include "mouse.h"

#include <SDL2/SDL_events.h>

#include <chrono>


namespace rterm {

namespace events {

struct EventType {
    enum {
        Quit = 0,      /**< User-requested quit */
        WindowShown,   /**< Window shown */
        WindowHidden,  /**< Window hidden */
        WindowMoved,   /**< Window moved */
        WindowResized, /**< Window resized */
        WindowExposed, /**< Window needs to be redrawn */
        System,        /**< System specific event */
        KeyDown,       /**< Key pressed */
        KeyUp,         /**< Key released */
        MouseMove,     /**< Mouse moved */
        MouseDown,     /**< Mouse button pressed */
        MouseUp,       /**< Mouse button released */
        MouseWheel,    /**< Mouse wheel motion */
        Unknown,       /**< Unknown event */
        COUNT          /**< Size of enum */
    };
};

inline int eventTypeFromSDLEvent(SDL_Event* event) noexcept;

class Event {
public:
    using rtclock = std::chrono::high_resolution_clock;

    explicit Event(SDL_Event* event = nullptr);
    explicit Event(int etype);
    virtual ~Event() = default;

    int type() const;
    const rtclock::time_point& time() const;
private:
    int type_;
    rtclock::time_point tp_;
};

class QuitEvent : public Event {
public:
    explicit QuitEvent()
        : Event(EventType::Quit) {}
};

class WindowEvent : public Event {
public:
    WindowEvent(SDL_Event* ev);

    int x() const;
    int y() const;
private:
    int x_;
    int y_;
};

class WindowShownEvent : public WindowEvent {
public:
    explicit WindowShownEvent(SDL_Event* ev = nullptr)
        : WindowEvent(ev) {
    }
};

class WindowHiddenEvent : public WindowEvent {
public:
    explicit WindowHiddenEvent(SDL_Event* ev = nullptr)
        : WindowEvent(ev) {
    }
};

class WindowExposedEvent : public WindowEvent {
public:
    explicit WindowExposedEvent(SDL_Event* ev = nullptr)
        : WindowEvent(ev) {
    }
};

class WindowResizedEvent : public WindowEvent {
public:
    explicit WindowResizedEvent(SDL_Event* ev = nullptr)
        : WindowEvent(ev) {
    }
};

class WindowMovedEvent : public WindowEvent {
public:
    explicit WindowMovedEvent(SDL_Event* ev = nullptr)
        : WindowEvent(ev) {
    }
};

class SystemEvent : public Event {
public:
    explicit SystemEvent(SDL_Event* event = nullptr)
        : Event(event) {
    }
private:
    SDL_Event sysevent_;
};

class KeyboardEvent : public Event {
public:
    explicit KeyboardEvent(SDL_Event* event = nullptr);

    const Key& key() const;
protected:
    Key key_;
};

class KeyDownEvent : public KeyboardEvent {
public:
    explicit KeyDownEvent(SDL_Event* event = nullptr,
                          SDL_Event* text = nullptr);
};

class KeyUpEvent : public KeyboardEvent {
public:
    explicit KeyUpEvent(SDL_Event* event = nullptr)
        : KeyboardEvent(event) {
    }
};

class MouseEvent : public Event {
public:
    explicit MouseEvent(SDL_Event* event = nullptr);

    uint32_t buttons() const;
private:
    uint32_t buttons_;
};

class MouseMoveEvent : public MouseEvent {
public:
    explicit MouseMoveEvent(SDL_Event* event = nullptr);

    int x() const;
    int y() const;
    int xrel() const;
    int yrel() const;

private:
    int x_;
    int y_;
    int xrel_;
    int yrel_;
};

class MouseDownEvent : public MouseEvent {
public:
    explicit MouseDownEvent(SDL_Event* event = nullptr);

    uint32_t button() const;
private:
    uint32_t button_;
};

class MouseUpEvent : public MouseEvent {
public:
    explicit MouseUpEvent(SDL_Event* event = nullptr);

    uint32_t button() const;
private:
    uint32_t button_;
};

class MouseWheelEvent : public Event {
public:
    explicit MouseWheelEvent(SDL_Event* event = nullptr);

    int dx() const;
    int dy() const;
private:
    int dx_;
    int dy_;
};

} // namespace events

} // namespace rterm
