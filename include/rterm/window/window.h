#pragma once

#include <rterm/terminal/options.h>

struct GLFWwindow;

namespace rterm::window {

class Window {
public:
    Window(const terminal::Options& opts);

    Window(Window&&) noexcept;
    Window& operator=(Window&&) noexcept;

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    ~Window() noexcept;

private:
    GLFWwindow* window_{nullptr};
    glfw::Loader loader_;
};

} // namespace rterm::window
