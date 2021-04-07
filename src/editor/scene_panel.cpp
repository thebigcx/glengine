#include "editor/scene_panel.h"
#include "engine/scene.h"
#include "engine/node.h"

#include "imgui/imgui.h"

void ScenePanel::set_scene(const std::shared_ptr<Scene>& scene)
{
    m_scene_context = scene;
}

void ScenePanel::render_node(Node* node)
{
    ImGui::PushID(node); // Memory location of node will be unique

    if (ImGui::TreeNodeEx(node->get_name().c_str(), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth))
    {
        for (auto& child : node->get_children())
            render_node(child);

        ImGui::TreePop();
    }

    if (ImGui::IsItemClicked())
    {
        m_selected_node = node;
    }

    ImGui::PopID();
}

void ScenePanel::imgui_render()
{
    ImGui::Begin("Scene Hierarchy");

    if (!m_scene_context.lock())
    {
        ImGui::End();
        return;
    }

    for (auto& node : m_scene_context.lock()->get_nodes())
        render_node(node);

    ImGui::End();
}