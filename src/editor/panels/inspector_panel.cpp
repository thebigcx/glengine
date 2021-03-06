#include "editor/panels/inspector_panel.h"
#include "editor/panels/scene_panel.h"
#include "engine/scene/scene.h"
#include "engine/scene/node.h"
#include "engine/renderer/sprite.h"
#include "engine/renderer/camera.h"
#include "engine/maths/vector2.h"
#include "engine/renderer/texture.h"
#include "engine/renderer/assets.h"
#include "engine/renderer/mesh.h"
#include "engine/renderer/mesh_renderer.h"
#include "engine/renderer/material.h"
#include "engine/renderer/shader.h"
#include "engine/audio/audio.h"
#include "engine/lua/lua_script.h"

#include "editor/fork_awesome/fork_awesome_icons.h"

#include "imgui/imgui.h"

void InspectorPanel::render_node_inspector()
{
    Node* node = node_selection;
    
    if (!node)
    {
        ImGui::Text("No game object selected");
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
        add_component<Sprite>(node, ICON_FK_PICTURE_O " Sprite");
        add_component<Camera>(node, ICON_FK_VIDEO_CAMERA " Camera");
        add_component<AudioSource>(node, ICON_FK_MUSIC " Audio Source");
        add_component<AudioListener>(node, ICON_FK_HEADPHONES " Audio Listener");
        add_component<LuaScript>(node, ICON_FK_CODE " Script");
        add_component<Mesh>(node, ICON_FK_CUBE " Mesh");
        add_component<MeshRenderer>(node, ICON_FK_CUBE " Mesh Renderer");

        ImGui::EndCombo();
    }
}

void InspectorPanel::render_material_inspector()
{
    std::weak_ptr<Material> material = AssetManager::get_instance()->get_material(material_selection);

    char buffer[128];
    strcpy(buffer, material.lock()->get_name().c_str());
    if (ImGui::InputText("##change_node_name", buffer, 128, ImGuiInputTextFlags_EnterReturnsTrue))
    {
        auto& list = AssetManager::get_instance()->get_material_cache().get_internal_list();

        std::shared_ptr<Material> old_mat = material.lock(); // Prevent from deleting
        list.erase(material.lock()->get_name());
        list.insert({ buffer, old_mat });

        material.lock()->set_name(std::string(buffer));
        material_selection = buffer;
    }

    if (ImGui::CollapsingHeader("Albedo"))
    {
        if (material.lock()->get_albedo().lock())
            ImGui::Image(reinterpret_cast<void*>(material.lock()->get_albedo().lock()->get_id()), ImVec2(50, 50), ImVec2(0, 1), ImVec2(1, 0));
        else
            ImGui::Image(0, ImVec2(50, 50), ImVec2(0, 1), ImVec2(1, 0));

        ImGui::SameLine();

        char buffer[128];
        if (material.lock()->get_albedo().lock())
            strcpy(buffer, material.lock()->get_albedo().lock()->get_path().c_str());
        
        ImGui::InputText("##texture_path", buffer, 128, ImGuiInputTextFlags_ReadOnly);

        if (ImGui::BeginDragDropTarget())
        {
            auto payload = ImGui::AcceptDragDropPayload("texture_asset");

            if (payload)
            {
                std::string path = (const char*)payload->Data;
                material.lock()->set_albedo(AssetManager::get_instance()->get_texture(path));
            }
        }
    }

    if (ImGui::CollapsingHeader("Shader"))
    {
        char buffer[128];
        if (material.lock()->get_shader().lock())
            strcpy(buffer, material.lock()->get_shader().lock()->get_path().c_str());
        
        ImGui::InputText("##shader_path", buffer, 128, ImGuiInputTextFlags_ReadOnly);

        if (ImGui::BeginDragDropTarget())
        {
            auto payload = ImGui::AcceptDragDropPayload("shader_asset");

            if (payload)
            {
                std::string path = (const char*)payload->Data;
                material.lock()->set_shader(AssetManager::get_instance()->get_shader(path));
            }
        }
    }
}

void InspectorPanel::render_texture_inspector()
{
    std::weak_ptr<Texture> texture = AssetManager::get_instance()->get_texture(texture_selection);

    bool is_srgb = texture.lock()->is_srgb();

    ImGui::Checkbox("Is SRGB", &is_srgb);
    if (is_srgb != texture.lock()->is_srgb())
    {
        // Reload texture
        texture.lock()->reload(texture.lock()->get_path(), is_srgb);
    }
}

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

    // TODO: use strategy design pattern
    
    if (selection_type == SelectionType::Node)
        render_node_inspector();
    else if (selection_type == SelectionType::Material)
        render_material_inspector();
    else if (selection_type == SelectionType::Texture)
        render_texture_inspector();

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

    bool open = ImGui::CollapsingHeader(name.c_str());

    bool deleted = false;
    if (ImGui::BeginPopupContextItem("Delete"))
    {
        if (ImGui::MenuItem(ICON_FK_TRASH " Delete"))
        {
            node->remove_component<T>();
            deleted = true;
        }

        ImGui::EndPopup();
    }

    if (open && !deleted)
        fn();

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

    draw_component<Sprite>(ICON_FK_PICTURE_O " Sprite", node, [node]
    {
        Sprite* sprite = node->get_component<Sprite>();

        Vector2f uv1 = sprite->get_uv1();
        Vector2f uv2 = sprite->get_uv2();

        bool use_uvs = sprite->using_uvs();

        ImGui::Columns(2);

        ImGui::Text("Texture");
        ImGui::NextColumn();

        char buffer[128];
        if (sprite->get_texture().lock())
            strcpy(buffer, sprite->get_texture().lock()->get_path().c_str());
        else
            strcpy(buffer, "");
        ImGui::InputText("##texture_path", buffer, 128, ImGuiInputTextFlags_ReadOnly);

        if (ImGui::BeginDragDropTarget())
        {
            auto payload = ImGui::AcceptDragDropPayload("texture_asset");

            if (payload)
            {
                std::string path = (const char*)payload->Data;
                sprite->set_texture(AssetManager::get_instance()->get_texture(path));
            }
        }
        
        ImGui::NextColumn();

        ImGui::Text("Use UVs");
        ImGui::NextColumn();
        ImGui::Checkbox("##use_uvs", &use_uvs);

        ImGui::Columns(1);
        ImGui::Text("UV Coordinates");
        ImGui::Columns(2);
        ImGui::Text("UV 1");
        ImGui::NextColumn();
        ImGui::InputFloat2("##uv1", &uv1.x);
        ImGui::NextColumn();
        ImGui::Text("UV 2");
        ImGui::NextColumn();
        ImGui::InputFloat2("##uv2", &uv2.x);
        ImGui::NextColumn();

        ImGui::Columns(1);

        sprite->set_uv1(uv1);
        sprite->set_uv2(uv2);

        sprite->use_uvs(use_uvs);
    });

    draw_component<Camera>(ICON_FK_VIDEO_CAMERA " Camera", node, [node]
    {
        Camera* camera = node->get_component<Camera>();
        bool main = camera->is_main_camera();
        
        ImGui::Columns(2);

        ImGui::Text("Main");
        ImGui::NextColumn();
        ImGui::Checkbox("##is_main_camera", &main);
        ImGui::NextColumn();

        if (main)
            Camera::set_main_camera(camera);
        else
        {
            if (Camera::main_camera == camera)
                Camera::set_main_camera(nullptr);
        }

        ImGui::Text("Projection");
        ImGui::NextColumn();

        std::string camera_types[] = { "Orthographic", "Perspective" };

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

    draw_component<LuaScript>(ICON_FK_CODE " Script", node, [node]
    {
        ImGui::Columns(2);

        ImGui::Text("Path");
        ImGui::NextColumn();

        char buffer[128];
        strcpy(buffer, node->get_component<LuaScript>()->get_path().c_str());

        ImGui::InputText("##script_path", buffer, 128, ImGuiInputTextFlags_ReadOnly);

        if (ImGui::BeginDragDropTarget())
        {
            auto payload = ImGui::AcceptDragDropPayload("script_asset");

            if (payload)
            {
                std::string path = (const char*)payload->Data;
                node->get_component<LuaScript>()->set_script(path);
            }
        }

        ImGui::NextColumn();

        ImGui::Columns(1);
        ImGui::Text("Global Variables");
        ImGui::Columns(2);

        unsigned int deleted_var = -1;
        int i = 0;
        for (auto& var : node->get_component<LuaScript>()->global_vars)
        {
            ImGui::PushID(&var);

            char buffer[128];
            strcpy(buffer, var.name.c_str());
            if (ImGui::InputText("##var_name", buffer, 128, ImGuiInputTextFlags_EnterReturnsTrue))
            {
                var.name = buffer;
            }

            if (ImGui::BeginPopupContextItem("Menu"))
            {
                if (ImGui::MenuItem(ICON_FK_TRASH_O " Delete"))
                    deleted_var = i;

                ImGui::EndPopup();
            }

            ImGui::NextColumn();

            strcpy(buffer, var.string.c_str());
            if (ImGui::InputText("##value", buffer, 128, ImGuiInputTextFlags_EnterReturnsTrue))
            {
                var.string = buffer;

                switch (var.type)
                {
                    case LuaGlobalVar::Type::Number:
                        var.number = std::stof(buffer);
                        break;
                    case LuaGlobalVar::Type::String:
                        var.string = buffer;
                        break;
                    case LuaGlobalVar::Type::Boolean:
                        var.boolean = std::stoi(buffer);
                        break;
                }
            }

            ImGui::NextColumn();

            ImGui::Text("Type");
            ImGui::NextColumn();

            std::string types[] = { "Number", "String", "Boolean", "Userdata" };

            if (ImGui::BeginCombo("##type", types[(int)var.type].c_str()))
            {
                for (int i = 0; i < 4; i++)
                {
                    bool selected = i == (int)var.type;
                    if (ImGui::Selectable(types[i].c_str(), &selected))
                    {
                        var.type = (LuaGlobalVar::Type)i;
                    }
                }

                ImGui::EndCombo();
            }

            ImGui::NextColumn();

            i++;

            ImGui::PopID();
        }

        if (deleted_var != -1)
        {
            node->get_component<LuaScript>()->global_vars.erase(node->get_component<LuaScript>()->global_vars.begin() + deleted_var);
        }

        ImGui::NextColumn();

        ImGui::Columns(1);

        if (ImGui::Button("Add Global Variable"))
        {
            node->get_component<LuaScript>()->global_vars.push_back(LuaGlobalVar("Var"));
        }
    });

    draw_component<AudioSource>(ICON_FK_MUSIC " Audio Source", node, [node]
    {
        ImGui::Columns(2);

        ImGui::Text("Path");
        ImGui::NextColumn();

        char buffer[128];
        if (buffer, node->get_component<AudioSource>()->get_buffer())
            strcpy(buffer, node->get_component<AudioSource>()->get_buffer()->get_path().c_str());

        ImGui::InputText("##audio_source_path", buffer, 128, ImGuiInputTextFlags_ReadOnly);

        if (ImGui::BeginDragDropTarget())
        {
            auto payload = ImGui::AcceptDragDropPayload("audio_buffer_asset");

            if (payload)
            {
                std::string path = (const char*)payload->Data;
                node->get_component<AudioSource>()->set_buffer(AssetManager::get_instance()->get_audio_buffer(path).lock());
            }
        }

        ImGui::NextColumn();

        ImGui::Columns(1);
    });

    draw_component<AudioListener>(ICON_FK_HEADPHONES " Audio Listener", node, [node]
    {
        bool main = node->get_component<AudioListener>()->is_main_listener();

        ImGui::Columns(2);

        ImGui::Text("Main");
        ImGui::NextColumn();
        ImGui::Checkbox("##is_main_listener", &main);
        ImGui::NextColumn();

        ImGui::Columns(1);

        if (main)
            AudioListener::set_main(*node->get_component<AudioListener>());
    });

    draw_component<Mesh>(ICON_FK_CUBE " Mesh", node, [node]
    {
        ImGui::Columns(2);

        ImGui::Text("Path");
        ImGui::NextColumn();

        char buffer[128];
        strcpy(buffer, node->get_component<Mesh>()->get_path().c_str());

        ImGui::InputText("##audio_source_path", buffer, 128, ImGuiInputTextFlags_ReadOnly);

        if (ImGui::BeginDragDropTarget())
        {
            auto payload = ImGui::AcceptDragDropPayload("model_asset");

            if (payload)
            {
                std::string path = (const char*)payload->Data;
                node->get_component<Mesh>()->load(path);
            }
        }

        ImGui::NextColumn();

        ImGui::Columns(1);
    });

    draw_component<MeshRenderer>(ICON_FK_CUBE " Mesh Renderer", node, [node]
    {
        ImGui::Columns(2);

        ImGui::Text("Material");
        ImGui::NextColumn();

        char buffer[128];
        if (node->get_component<MeshRenderer>()->get_material().lock())
            strcpy(buffer, node->get_component<MeshRenderer>()->get_material().lock()->get_name().c_str());

        ImGui::InputText("##material_name", buffer, 128, ImGuiInputTextFlags_ReadOnly);

        if (ImGui::BeginDragDropTarget())
        {
            auto payload = ImGui::AcceptDragDropPayload("material_asset");

            if (payload)
            {
                std::string name = (const char*)payload->Data;
                node->get_component<MeshRenderer>()->set_material(AssetManager::get_instance()->get_material(name));
            }
        }

        ImGui::NextColumn();

        ImGui::Columns(1);
    });
}