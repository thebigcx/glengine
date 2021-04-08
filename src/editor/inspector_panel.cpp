#include "editor/inspector_panel.h"
#include "editor/scene_panel.h"
#include "engine/scene.h"
#include "engine/node.h"
#include "engine/sprite.h"
#include "engine/camera.h"

#include "editor/fork_awesome/fork_awesome_icons.h"

#include "imgui/imgui.h"

template<typename T>
void InspectorPanel::add_component(Node* node, const std::string& name)
{
    if (!node->has_component<T>())
    {
        if (ImGui::Selectable(name.c_str()))
        {
            node->create_component<T>();
        }
    }
}

void InspectorPanel::set_scene(const std::shared_ptr<Scene>& scene)
{
    m_scene_context = scene;
}

void InspectorPanel::imgui_render()
{
    ImGui::Begin(ICON_FK_CUBE " Inspector");
    
    Node* node = ScenePanel::get_selected_node();
    
    if (!node)
    {
        ImGui::Text("No game object selected");
        ImGui::End();
        return;
    }

    bool node_enabled = node->is_enabled();
    ImGui::Checkbox("##node_enabled", &node_enabled);
    node->set_enabled(node_enabled);

    ImGui::SameLine();

    char buffer[128];
    strcpy(buffer, node->get_name().c_str());
    if (ImGui::InputText("##change_node_name", buffer, 128, ImGuiInputTextFlags_EnterReturnsTrue))
    {
        node->set_name(std::string(buffer));
    }

    draw_components(node);

    if (ImGui::BeginCombo("##add_component", ICON_FK_PLUS " Add Component", ImGuiComboFlags_NoArrowButton))
    {
        add_component<Sprite>(node, "Sprite");

        ImGui::EndCombo();
    }

    ImGui::End();
}

template<typename T, typename F>
void InspectorPanel::draw_component(const std::string& name, Node* node, const F& fn)
{
    if (!node)
        return;

    if (!node->has_component<T>())
        return;

    ImGui::PushID(name.c_str());

    if (ImGui::CollapsingHeader(name.c_str()))
        fn();

    if (ImGui::BeginPopupContextItem("Delete"))
    {
        if (ImGui::MenuItem(ICON_FK_TRASH " Delete"))
            node->remove_component<T>();

        ImGui::EndPopup();
    }

    ImGui::PopID();
}

void InspectorPanel::draw_components(Node* node)
{
    if (ImGui::CollapsingHeader(ICON_FK_CROSSHAIRS " Transform"))
    {
        ImGui::Columns(2);

        ImGui::Text("Translation");
        ImGui::NextColumn();
        Vector3f tr = node->get_transform().get_translation();
        ImGui::DragFloat3("##translation", &tr.x, 0.1f);
        node->get_transform().set_translation(tr);
        ImGui::NextColumn();

        ImGui::Text("Scale");
        ImGui::NextColumn();
        Vector3f sc = node->get_transform().get_scale();
        ImGui::DragFloat3("##scale", &sc.x, 0.1f);
        node->get_transform().set_scale(sc);
        ImGui::NextColumn();

        ImGui::Text("Rotation");
        ImGui::NextColumn();
        Vector3f ro = node->get_transform().get_rotation();
        ImGui::DragFloat3("##rotation", &ro.x, 0.1f);
        node->get_transform().set_rotation(ro);
        ImGui::NextColumn();

        ImGui::Columns(1);
    }

    draw_component<Sprite>(ICON_FK_PICTURE_O " Sprite", node, []
    {

    });

    draw_component<Camera>(ICON_FK_VIDEO_CAMERA " Camera", node, [node]
    {
        ImGui::Columns(2);

        ImGui::Text("Projection");
        ImGui::NextColumn();

        std::string camera_types[] = { "Orthographic", "Perspective" };

        Camera* camera = node->get_component<Camera>();

        if (ImGui::BeginCombo("##camera_type", camera_types[(int)camera->get_projection_type()].c_str()))
        {
            if (ImGui::Selectable(camera_types[0].c_str()))
            {
                camera->set_projection_type(CameraProjection::Orthographic);
            }
            if (ImGui::Selectable(camera_types[1].c_str()))
            {
                camera->set_projection_type(CameraProjection::Perspective);
            }

            ImGui::EndCombo();
        }

        ImGui::NextColumn();

        if (camera->get_projection_type() == CameraProjection::Orthographic)
        {
            float size = camera->get_ortho().size;
            float near = camera->get_ortho().near_clip;
            float far = camera->get_ortho().far_clip;

            ImGui::Text("Size");
            ImGui::NextColumn();
            ImGui::InputFloat("##size", &size);
            ImGui::NextColumn();
            ImGui::Columns(1);
            ImGui::Text("Clipping Planes");
            ImGui::Columns(4);
            ImGui::Text("Near");
            ImGui::NextColumn();
            ImGui::InputFloat("##near", &near);
            ImGui::NextColumn();
            ImGui::Text("Far");
            ImGui::NextColumn();
            ImGui::InputFloat("##far", &far);
            ImGui::NextColumn();
            ImGui::Columns(2);

            camera->set_ortho(size, near, far);
        }
        else
        {
            float fov = camera->get_perspective().fov;
            float near = camera->get_perspective().near_clip;
            float far = camera->get_perspective().far_clip;

            ImGui::Text("FOV");
            ImGui::NextColumn();
            ImGui::SliderFloat("##fov", &fov, 10, 180);
            ImGui::NextColumn();
            ImGui::Columns(1);
            ImGui::Text("Clipping Planes");
            ImGui::Columns(4);
            ImGui::Text("Near");
            ImGui::NextColumn();
            ImGui::InputFloat("##near", &near);
            ImGui::NextColumn();
            ImGui::Text("Far");
            ImGui::NextColumn();
            ImGui::InputFloat("##far", &far);
            ImGui::NextColumn();
            ImGui::Columns(2);

            camera->set_perspective(fov, near, far);
        }

        ImGui::Columns(1);
    });
}