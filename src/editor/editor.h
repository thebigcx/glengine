#pragma once

#include "engine/core/application.h"
#include "engine/maths/vector2.h"
#include "editor/editor_camera.h"

class Scene;
class Camera;
class Texture;
class Framebuffer;

class Editor : public Application
{
public:
    Editor();

    void on_start() override;
    void on_update(float dt) override;
    void on_destroy() override;
    void on_event(Event& e) override;

    void open_scene(const std::string& name);

private:
    std::shared_ptr<Framebuffer> m_framebuffer;

    EditorCamera m_camera;

    Vector2u m_viewport_size; // To test changes of framebuffer size

    bool m_is_playing = false;

    int m_imguizmo_operation = 0;

    void render_menu_bar();
};