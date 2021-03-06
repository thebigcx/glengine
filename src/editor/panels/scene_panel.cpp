#include "editor/panels/scene_panel.h"
#include "editor/panels/inspector_panel.h"
#include "engine/scene/scene.h"
#include "engine/scene/node.h"
#include "engine/renderer/sprite.h"
#include "engine/renderer/camera.h"
#include "engine/renderer/mesh.h"
#include "engine/lua/lua_script.h"
#include "engine/renderer/mesh_renderer.h"
#include "engine/audio/audio.h"

#include "editor/fork_awesome/fork_awesome_icons.h"

#include "imgui/imgui.h"

void ScenePanel::set_scene(const std::shared_ptr<Scene>& scene)
{
    m_scene_context = scene;
}

void ScenePanel::render_node(Node* node)
{
    ImGui::PushID(node); // Memory location of node will be unique

    std::string icon = "";

    if (node->has_component<Camera>())
        icon = ICON_FK_VIDEO_CAMERA;
    else if (node->has_component<Sprite>())
        icon = ICON_FK_PICTURE_O;
    else if (node->has_component<Mesh>() || node->has_component<MeshRenderer>())
        icon = ICON_FK_CUBE;
    else if (node->has_component<AudioListener>())
        icon = ICON_FK_HEADPHONES;
    else if (node->has_component<AudioSource>())
        icon = ICON_FK_MUSIC;
    else if (node->has_component<LuaScript>())
        icon = ICON_FK_CODE;

    bool is_open = ImGui::TreeNodeEx((icon + " " + node->get_name()).c_str(), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth);

    if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
    {
        InspectorPanel::selection_type = SelectionType::Node;
        InspectorPanel::node_selection = node;
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

    for (auto& node : m_scene_context.lock()->get_root_node()->get_children())
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
        InspectorPanel::node_selection = nullptr;

        if (ImGui::BeginMenu(ICON_FK_PLUS " Create"))
        {
            // TODO: this is shit
            if (ImGui::MenuItem(ICON_FK_FILE " Empty Game Object"))
            {
                auto object = m_scene_context.lock()->create_node("Game Object");
                InspectorPanel::selection_type = SelectionType::Node;
                InspectorPanel::node_selection = object;
            }
            if (ImGui::MenuItem(ICON_FK_VIDEO_CAMERA " Camera"))
            {
                auto object = m_scene_context.lock()->create_node("Camera");
                object->create_component<Camera>();
                if (!Camera::main_camera)
                    Camera::set_main_camera(object->get_component<Camera>());

                InspectorPanel::selection_type = SelectionType::Node;
                InspectorPanel::node_selection = object;
            }
            if (ImGui::MenuItem(ICON_FK_PICTURE_O " Sprite"))
            {
                auto object = m_scene_context.lock()->create_node("Sprite");
                object->create_component<Sprite>();
                InspectorPanel::selection_type = SelectionType::Node;
                InspectorPanel::node_selection = object;
            }
            if (ImGui::MenuItem(ICON_FK_MUSIC " Audio Source"))
            {
                auto object = m_scene_context.lock()->create_node("Audio Source");
                object->create_component<AudioSource>();
                InspectorPanel::selection_type = SelectionType::Node;
                InspectorPanel::node_selection = object;
            }
            if (ImGui::MenuItem(ICON_FK_CUBE " Mesh"))
            {
                auto object = m_scene_context.lock()->create_node("Mesh");
                object->create_component<Mesh>();
                object->create_component<MeshRenderer>();
                InspectorPanel::selection_type = SelectionType::Node;
                InspectorPanel::node_selection = object;
            }
            if (ImGui::MenuItem(ICON_FK_HEADPHONES " Audio Listener"))
            {
                auto object = m_scene_context.lock()->create_node("Audio Listener");
                object->create_component<AudioListener>();
                InspectorPanel::selection_type = SelectionType::Node;
                InspectorPanel::node_selection = object;
            }

            ImGui::EndMenu();
        }

        ImGui::EndPopup();
    }

    ImGui::End();
}