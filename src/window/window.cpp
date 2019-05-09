#include <rterm/window/window.h>

#include <GLFW/glfw3.h>

namespace rterm::window {

Window::Window(const terminal::Options& opts) {
}

Window::~Window() {
    if (window_) {
        glfwDestroyWindow(window_);
    }
}

} // namespace rterm::window
