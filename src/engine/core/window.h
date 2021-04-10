#pragma once

#include <cstdint>
#include <string>

struct GLFWwindow;

class Window
{
public:
    Window(uint32_t width, uint32_t height, const std::string& title);
    ~Window();

    void poll_events() const;
    void swap_buffers() const;

    bool should_close() const;

    GLFWwindow* get_native_handle() const { return m_window_handle; }

    uint32_t get_width() const { return m_width; }
    uint32_t get_height() const { return m_height; }

private:
    GLFWwindow* m_window_handle = nullptr;

    uint32_t m_width, m_height;
    std::string m_title;
};