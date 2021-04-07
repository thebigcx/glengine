#pragma once

#include <memory>

#include "engine/window.h"

class Window;

class Application
{
public:
    void run();

    virtual void on_start() = 0;
    virtual void on_update(float dt) = 0;
    virtual void on_destroy() = 0;

    const std::unique_ptr<Window>& get_window() const { return m_window; }
    static Application* get_instance() { return m_instance; }

protected:
    Application();

    bool m_is_running = true;

private:
    static inline Application* m_instance = nullptr;

    std::unique_ptr<Window> m_window = nullptr;
};