#include "engine/application.h"
#include "engine/renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

Application::Application()
{
    m_instance = this;
}

void Application::run()
{
    m_window = std::make_unique<Window>(1920, 1080, "Application");
    Renderer::init();

    on_start();

    while (m_is_running)
    {
        m_window->poll_events();
        if (m_window->should_close())
        {
            m_is_running = false;
        }

        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        on_update(1);

        m_window->swap_buffers();
    }

    on_destroy();
}