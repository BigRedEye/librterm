#pragma once

#include "rterm/window/key.h"
#include "rterm/window/mouse.h"
#include "rterm/util/util.h"

#include <SDL_events.h>

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

inline i32 eventTypeFromSDLEvent(SDL_Event* event) noexcept;

class Event {
public:
    using rtclock = std::chrono::high_resolution_clock;

    explicit Event(SDL_Event* event = nullptr);
    explicit Event(i32 etype);
    virtual ~Event() = default;

    i32 type() const;
    const rtclock::time_point& time() const;

private:
    i32 type_;
    rtclock::time_point tp_;
};

class QuitEvent : public Event {
public:
    explicit QuitEvent()
        : Event(EventType::Quit) {
    }
};

class WindowEvent : public Event {
public:
    WindowEvent(SDL_Event* ev);

    i32 x() const;
    i32 y() const;

private:
    i32 x_;
    i32 y_;
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
    explicit KeyDownEvent(SDL_Event* event = nullptr, SDL_Event* text = nullptr);
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

    ui32 buttons() const;

private:
    ui32 buttons_;
};

class MouseMoveEvent : public MouseEvent {
public:
    explicit MouseMoveEvent(SDL_Event* event = nullptr);

    i32 x() const;
    i32 y() const;
    i32 xrel() const;
    i32 yrel() const;

private:
    i32 x_;
    i32 y_;
    i32 xrel_;
    i32 yrel_;
};

class MouseDownEvent : public MouseEvent {
public:
    explicit MouseDownEvent(SDL_Event* event = nullptr);

    ui32 button() const;

private:
    ui32 button_;
};

class MouseUpEvent : public MouseEvent {
public:
    explicit MouseUpEvent(SDL_Event* event = nullptr);

    ui32 button() const;

private:
    ui32 button_;
};

class MouseWheelEvent : public Event {
public:
    explicit MouseWheelEvent(SDL_Event* event = nullptr);

    i32 dx() const;
    i32 dy() const;

private:
    i32 dx_;
    i32 dy_;
};

} // namespace events

} // namespace rterm
