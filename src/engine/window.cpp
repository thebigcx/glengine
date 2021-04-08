#include "engine/window.h"
#include "engine/glad/glad.h"

#include <GLFW/glfw3.h>

Window::Window(uint32_t width, uint32_t height, const std::string& title)
    : m_width(width), m_height(height), m_title(title)
{
    glfwInit();
    m_window_handle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    glfwSwapInterval(1);
    glfwMakeContextCurrent(m_window_handle);

    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glViewport(0, 0, width, height);
}

Window::~Window()
{
    glfwDestroyWindow(m_window_handle);
}

void Window::poll_events() const
{
    glfwPollEvents();
}

void Window::swap_buffers() const
{
    glfwSwapBuffers(m_window_handle);
}

bool Window::should_close() const
{
    return glfwWindowShouldClose(m_window_handle);
}