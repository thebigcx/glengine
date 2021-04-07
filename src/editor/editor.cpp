#include "editor/editor.h"
#include "editor/imgui_layer.h"
#include "engine/scene.h"
#include "engine/node.h"
#include "engine/sprite.h"

#include "editor/scene_panel.h"
#include "editor/inspector_panel.h"

#include "imgui/imgui.h"

Editor::Editor()
{

}

void Editor::on_start()
{
    m_current_scene = std::make_shared<Scene>();
    auto node = m_current_scene->create_node("Test");
    node->create_child("Child");
    node->create_component<Sprite>();

    ImGuiLayer::init();
    ScenePanel::set_scene(m_current_scene);
    InspectorPanel::set_scene(m_current_scene);
}

void Editor::on_update(float dt)
{
    ImGuiLayer::begin_frame();

    ScenePanel::imgui_render();
    InspectorPanel::imgui_render();

    ImGui::ShowDemoWindow();
    ImGuiLayer::end_frame();
}

void Editor::on_destroy()
{
    ImGuiLayer::destroy();
}