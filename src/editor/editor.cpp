#include "editor/editor.h"
#include "editor/imgui_layer.h"
#include "engine/scene.h"
#include "engine/node.h"
#include "engine/sprite.h"
#include "engine/renderer_2d.h"
#include "engine/camera.h"
#include "engine/event.h"
#include "engine/texture.h"
#include "engine/framebuffer.h"

#include "editor/scene_panel.h"
#include "editor/inspector_panel.h"
#include "editor/assets_panel.h"

#include "imgui/imgui.h"

#include <glad/glad.h>

Editor::Editor()
{
    
}

void Editor::on_start()
{
    m_current_scene = std::make_shared<Scene>();

    // TEST
    auto node = m_current_scene->create_node("Test");
    node->create_child("Child");
    node->create_component<Sprite>();
    auto cam_node = m_current_scene->create_node("Camera");
    auto camera = cam_node->create_component<Camera>();
    Camera::set_main_camera(camera);
    node->get_transform().set_scale(Vector3f(1, 1, 1));

    ImGuiLayer::init();
    ScenePanel::set_scene(m_current_scene);
    InspectorPanel::set_scene(m_current_scene);


    // TODO: fix this. Make a function.
    m_framebuffer = std::make_shared<Framebuffer>(Application::get_instance()->get_window()->get_width(), Application::get_instance()->get_window()->get_height());
    m_camera.on_viewport_resize(Application::get_instance()->get_window()->get_width(), Application::get_instance()->get_window()->get_height());
}

void Editor::on_update(float dt)
{
    m_camera.on_update(dt);

    m_current_scene->on_update(dt);

    m_framebuffer->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_current_scene->on_editor_render(m_camera);
    m_framebuffer->unbind();


    ImGuiLayer::begin_frame();

    auto viewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking
    | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
	| ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::Begin("Window", nullptr, windowFlags);
    ImGui::DockSpace(ImGui::GetID("MyDockSpace"), ImVec2(0.f, 0.f), ImGuiDockNodeFlags_None);

    ScenePanel::imgui_render();
    InspectorPanel::imgui_render();
    AssetsPanel::imgui_render();

    ImGui::Begin("Viewport");

    m_camera.set_focus(ImGui::IsWindowFocused());

    ImVec2 size = ImGui::GetContentRegionAvail();

    if (size.x != m_viewport_size.x || size.y != m_viewport_size.y)
    {
        m_viewport_size = Vector2u(size.x, size.y);
        m_framebuffer->resize(size.x, size.y);
        m_camera.on_viewport_resize(size.x, size.y);
        WindowResizeEvent e(size.x, size.y);
        m_current_scene->on_event(e);
    }
    
    ImGui::Image(reinterpret_cast<void*>(m_framebuffer->get_color_texture()), size, ImVec2{0, 1}, ImVec2{1, 0});

    ImGui::End();

    ImGui::End();
    ImGui::PopStyleVar();

    ImGuiLayer::end_frame();
}

void Editor::on_destroy()
{
    ImGuiLayer::destroy();
}

void Editor::on_event(Event& e)
{
    if (e.get_type() != EventType::WindowResize) // Viewport is different to window with ImGui
    {
        m_current_scene->on_event(e);
    }
}