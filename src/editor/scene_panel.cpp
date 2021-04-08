#include "editor/scene_panel.h"
#include "engine/scene.h"
#include "engine/node.h"
#include "engine/sprite.h"
#include "engine/camera.h"

#include "editor/fork_awesome/fork_awesome_icons.h"

#include "imgui/imgui.h"

void ScenePanel::set_scene(const std::shared_ptr<Scene>& scene)
{
    m_scene_context = scene;
}

// TODO: doesn't quite work
void ScenePanel::render_node(Node* node)
{
    ImGui::PushID(node); // Memory location of node will be unique

    std::string icon = "";

    if (node->has_component<Camera>())
        icon = ICON_FK_VIDEO_CAMERA;
    if (node->has_component<Sprite>())
        icon = ICON_FK_PICTURE_O;
    

    if (ImGui::TreeNodeEx((icon + " " + node->get_name()).c_str(), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth))
    {
        for (auto& child : node->get_children())
            render_node(child);

        ImGui::TreePop();
    }

    if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
    {
        m_selected_node = node;
    }

    if (ImGui::BeginPopupContextItem("node_context_menu"))
    {
        if (ImGui::BeginMenu(ICON_FK_PLUS " Create Child"))
        {

            ImGui::EndMenu();
            node->create_child("New Child");
        }
        if (ImGui::MenuItem(ICON_FK_TRASH " Delete"))
        {
            m_node_to_delete = node;
        }

        ImGui::EndPopup();
    }

    ImGui::PopID();
}

void ScenePanel::imgui_render()
{
    ImGui::Begin(ICON_FK_TREE " Scene Hierarchy");

    if (!m_scene_context.lock())
    {
        ImGui::End();
        return;
    }

    for (auto& node : m_scene_context.lock()->get_nodes())
        render_node(node);

    if (m_node_to_delete)
    {
        m_node_to_delete = nullptr;
    }

    if (ImGui::BeginPopupContextWindow(0, 1, false))
    {
        m_selected_node = nullptr;

        if (ImGui::BeginMenu(ICON_FK_PLUS " Create"))
        {
            if (ImGui::MenuItem(ICON_FK_FILE " Empty Game Object"))
            {
                auto object = m_scene_context.lock()->create_node("Game Object");
                m_selected_node = object;
            }
            if (ImGui::MenuItem(ICON_FK_VIDEO_CAMERA " Camera"))
            {
                auto object = m_scene_context.lock()->create_node("Camera");
                object->create_component<Camera>();
                m_selected_node = object;
            }
            if (ImGui::MenuItem(ICON_FK_PICTURE_O " Sprite"))
            {
                auto object = m_scene_context.lock()->create_node("Sprite");
                object->create_component<Sprite>();
                m_selected_node = object;
            }

            ImGui::EndMenu();
        }

        ImGui::EndPopup();
    }

    ImGui::End();
}