#include "editor/panels/scene_panel.h"
#include "engine/scene/scene.h"
#include "engine/scene/node.h"
#include "engine/renderer/sprite.h"
#include "engine/renderer/camera.h"
#include "engine/audio/audio.h"

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
    
    bool is_open = ImGui::TreeNodeEx((icon + " " + node->get_name()).c_str(), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth);

    if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
    {
        m_selected_node = node;
    }

    if (ImGui::BeginPopupContextItem("node_context_menu"))
    {
        if (ImGui::MenuItem(ICON_FK_PLUS " Create Child"))
        {
            node->create_child("New Child");
        }
        if (ImGui::MenuItem(ICON_FK_TRASH " Delete"))
        {
            m_node_to_delete = node;
        }

        ImGui::EndPopup();
    }

    if (is_open)
    {
        for (auto& child : node->get_children())
            render_node(child);

        ImGui::TreePop();
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
        if (m_node_to_delete->get_parent())
        {
            m_node_to_delete->get_parent()->remove_child(m_node_to_delete);
        }
        else
        {
            m_scene_context.lock()->remove_node(m_node_to_delete);
        }

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
            if (ImGui::MenuItem(ICON_FK_PICTURE_O " Audio Source"))
            {
                auto object = m_scene_context.lock()->create_node("Audio Source");
                object->create_component<AudioSource>();
                m_selected_node = object;
            }

            ImGui::EndMenu();
        }

        ImGui::EndPopup();
    }

    ImGui::End();
}