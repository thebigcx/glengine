#include "engine/core/application.h"
#include "engine/renderer/renderer.h"
#include "engine/core/event.h"
#include "engine/audio/audio.h"
#include "engine/renderer/assets.h"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

Application::Application()
{
    m_instance = this;
}

void Application::run(const std::string& project)
{
    m_window = std::make_unique<Window>(3840, 2160, "Application");
    Renderer::init();
    Audio::init();
    AssetManager::get_instance()->load_asset_folder(project); // TODO: asset folder from command line (maybe?)

    on_start();

    // Send an initial resize event to configure everything
    WindowResizeEvent e(m_window->get_width(), m_window->get_height());
    on_event(e);

    glClearColor(0, 0, 0, 1);

    while (m_is_running)
    {
        m_window->poll_events();
        if (m_window->should_close())
        {
            m_is_running = false;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        on_update(1);

        m_window->swap_buffers();
    }

    on_destroy();

    AssetManager::get_instance()->flush();

    Audio::finalize();
}