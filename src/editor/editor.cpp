#include "editor/editor.h"
#include "editor/imgui_layer.h"
#include "engine/scene.h"
#include "engine/node.h"
#include "engine/sprite.h"
#include "engine/renderer_2d.h"
#include "engine/camera.h"
#include "engine/event.h"
#include "engine/texture.h"

#include "editor/scene_panel.h"
#include "editor/inspector_panel.h"

#include "imgui/imgui.h"

#include <iostream>

Editor::Editor()
{

}

void Editor::on_start()
{
    m_current_scene = std::make_shared<Scene>();
    auto node = m_current_scene->create_node("Test");
    node->create_child("Child");
    node->create_component<Sprite>();
    m_camera = node->create_component<Camera>();

    ImGuiLayer::init();
    ScenePanel::set_scene(m_current_scene);
    InspectorPanel::set_scene(m_current_scene);

    m_texture = std::make_shared<Texture>("assets/texture_test.png");
}

void Editor::on_update(float dt)
{
    // TEST
    WindowResizeEvent event(Application::get_instance()->get_window()->get_width(), Application::get_instance()->get_window()->get_height());
    m_current_scene->on_event(event);
    m_camera->on_transform_change();

    Renderer2D::start_scene(*m_camera);
    //Renderer2D::render_quad(Vector2f(0, 0), Vector2f(100, 100), Vector3f(1, 0, 0));
    Renderer2D::render_sprite(m_texture, Vector2f(m_x++, 0), Vector2f(100, 100), Vector3f(1, 1, 1), Vector2f(0, 0), Vector2f(256, 256));
    Renderer2D::flush_batch();


    ImGuiLayer::begin_frame();

    auto viewport = ImGui::GetMainViewport();

    /*ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking
    | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
	| ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::Begin("Window", nullptr, windowFlags);
    ImGui::DockSpace(ImGui::GetID("MyDockSpace"), ImVec2(0.f, 0.f), ImGuiDockNodeFlags_None);*/

    ScenePanel::imgui_render();
    InspectorPanel::imgui_render();

    //ImGui::End();
    //ImGui::PopStyleVar();

    ImGui::ShowDemoWindow();
    ImGuiLayer::end_frame();
}

void Editor::on_destroy()
{
    ImGuiLayer::destroy();
}