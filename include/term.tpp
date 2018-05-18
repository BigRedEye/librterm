#pragma once

#include "term.h"
#include "event.h"

namespace rterm {

using namespace events;

template<typename F>
inline void Term::onKeyDown(F&& callback) {
    eventSystem_.registerCallback(EventType::KeyDown, [&](Event* ev){
        events::KeyDownEvent* keyev =
                static_cast<events::KeyDownEvent*>(ev);
        callback(*keyev);
    });
}

template<typename F>
void Term::onKeyUp(F&& callback) {
    eventSystem_.registerCallback(EventType::KeyUp, [&](Event* ev){
        events::KeyUpEvent* keyev =
                static_cast<events::KeyUpEvent*>(ev);
        callback(*keyev);
    });
}

template<typename F>
void Term::onMouseMove(F&& callback) {
    eventSystem_.registerCallback(EventType::MouseMove, [&](Event* ev){
        events::MouseMoveEvent* event =
                static_cast<events::MouseMoveEvent*>(ev);
        callback(*event);
    });
}

template<typename F>
void Term::onMouseDown(F&& callback) {
    eventSystem_.registerCallback(EventType::MouseDown, [&](Event* ev){
        events::MouseDownEvent* event =
                static_cast<events::MouseDownEvent*>(ev);
        callback(*event);
    });
}

template<typename F>
void Term::onMouseUp(F&& callback) {
    eventSystem_.registerCallback(EventType::MouseUp, [&](Event* ev){
        events::MouseUpEvent* event =
                static_cast<events::MouseUpEvent*>(ev);
        callback(*event);
    });
}

template<typename F>
void Term::onMouseWheel(F&& callback) {
    eventSystem_.registerCallback(EventType::MouseWheel, [&](Event* ev){
        events::MouseWheelEvent* event =
                static_cast<events::MouseWheelEvent*>(ev);
        callback(*event);
    });
}

template<typename F>
void Term::onWindowResized(F&& callback) {
    eventSystem_.registerCallback(EventType::WindowResized, [&](Event* ev){
        events::WindowResizedEvent* event =
                static_cast<events::WindowResizedEvent*>(ev);
        callback(*event);
    });
}

template<typename F>
void Term::onWindowMoved(F&& callback) {
    eventSystem_.registerCallback(EventType::WindowMoved, [&](Event* ev){
        events::WindowMovedEvent* event =
                static_cast<events::WindowMovedEvent*>(ev);
        callback(*event);
    });
}

template<typename F>
void Term::onWindowShown(F&& callback) {
    eventSystem_.registerCallback(EventType::WindowShown, [&](Event* ev){
        events::WindowShownEvent* event =
                static_cast<events::WindowShownEvent*>(ev);
        callback(*event);
    });
}

template<typename F>
void Term::onWindowHidden(F&& callback) {
    eventSystem_.registerCallback(EventType::WindowHidden, [&](Event* ev){
        events::WindowHiddenEvent* event =
                static_cast<events::WindowHiddenEvent*>(ev);
        callback(*event);
    });
}

}
