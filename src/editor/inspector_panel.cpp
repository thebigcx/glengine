#include "editor/inspector_panel.h"
#include "editor/scene_panel.h"
#include "engine/scene.h"
#include "engine/node.h"
#include "engine/sprite.h"

#include "imgui/imgui.h"

void InspectorPanel::set_scene(const std::shared_ptr<Scene>& scene)
{
    m_scene_context = scene;
}

void InspectorPanel::imgui_render()
{
    ImGui::Begin("Inspector");
    
    Node* node = ScenePanel::get_selected_node();
    
    if (!node)
    {
        ImGui::End();
        return;
    }

    if (node->has_component<Sprite>())
    {
        ImGui::CollapsingHeader("Sprite");
    }

    ImGui::End();
}