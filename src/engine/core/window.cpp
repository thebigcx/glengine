#include "engine/core/window.h"
#include "engine/glad/glad.h"
#include "engine/core/application.h"
#include "engine/core/event.h"

#include <GLFW/glfw3.h>

static void framebuffer_size_callback(GLFWwindow* window_handle, int width, int height)
{
    Application* app = (Application*)glfwGetWindowUserPointer(window_handle);
    WindowResizeEvent event(width, height);
    app->on_event(event);
}

static void key_callback(GLFWwindow* window_handle, int key, int scancode, int action, int mods)
{
    Application* app = (Application*)glfwGetWindowUserPointer(window_handle);
    KeyPressEvent event(static_cast<Key>(key));
    app->on_event(event);
}

Window::Window(uint32_t width, uint32_t height, const std::string& title)
    : m_width(width), m_height(height), m_title(title)
{
    glfwInit();
    m_window_handle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    glfwSwapInterval(1);
    glfwMakeContextCurrent(m_window_handle);

    glfwSetWindowUserPointer(m_window_handle, Application::get_instance());

    glfwSetFramebufferSizeCallback(m_window_handle, framebuffer_size_callback);
    glfwSetKeyCallback(m_window_handle, key_callback);

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