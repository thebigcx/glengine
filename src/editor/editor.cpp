#include "editor/editor.h"
#include "editor/imgui_layer.h"
#include "engine/scene/scene.h"
#include "engine/scene/node.h"
#include "engine/renderer/sprite.h"
#include "engine/renderer/renderer_2d.h"
#include "engine/renderer/camera.h"
#include "engine/core/event.h"
#include "engine/renderer/texture.h"
#include "engine/renderer/framebuffer.h"
#include "engine/renderer/mesh.h"
#include "engine/renderer/mesh_renderer.h"
#include "engine/renderer/material.h"
#include "engine/renderer/assets.h"
#include "engine/lua/lua_script.h"
#include "engine/audio/audio.h"
#include "engine/core/serializer.h"
#include "engine/core/deserializer.h"

#include "editor/panels/scene_panel.h"
#include "editor/panels/inspector_panel.h"
#include "editor/panels/assets_panel.h"

#include "imgui/imgui.h"
#include "editor/imguizmo/ImGuizmo.h"
#include "editor/fork_awesome/fork_awesome_icons.h"

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
    node->create_component<LuaScript>("assets/script3.lua");
    node->create_component<Mesh>();

    auto mrender = node->create_component<MeshRenderer>();

    node->get_component<Mesh>()->load("assets/test.fbx");
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
    if (m_is_playing)
    {
        auto& window = Application::get_instance()->get_window();
        glViewport(0, 0, window->get_width(), window->get_height());

        m_current_scene->on_update(dt);
        m_current_scene->on_render();
        
        if (Keyboard::is_key_pressed(Key::Escape))
        {
            m_is_playing = false;
            m_current_scene->on_destroy();
            // TODO: Need to make a copy and restore it here
        }

        return;
    }

    m_current_scene->on_editor_update(dt);
    m_camera.on_update(dt);

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

    render_menu_bar();

    ScenePanel::imgui_render();
    InspectorPanel::imgui_render();
    AssetsPanel::imgui_render();

    ImGui::Begin("ScenePlay");
    if (ImGui::Button(ICON_FK_PLAY))
    {
        m_is_playing = true;
        m_current_scene->on_start();
    }
    ImGui::End();

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

    // IMGUIZMO

    if (InspectorPanel::selection_type == SelectionType::Node && InspectorPanel::node_selection)
    {
        ImGuizmo::SetOrthographic(true);
        ImGuizmo::SetDrawlist();

        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

        const Matrix4f& view = m_camera.get_view_matrix();
        const Matrix4f& projection = m_camera.get_projection_matrix();
        Matrix4f transform = InspectorPanel::node_selection->get_transform().get_local_transform();

        ImGuizmo::Manipulate(&view[0].x, &projection[0].x, (ImGuizmo::OPERATION)m_imguizmo_operation, ImGuizmo::LOCAL, &transform[0].x, nullptr);

        if (ImGuizmo::IsUsing())
        {
            Vector3f translation, rotation, scale;
            Matrix4f::decompose_transform(transform, translation, rotation, scale);

            InspectorPanel::node_selection->get_transform().set_translation(translation);
            InspectorPanel::node_selection->get_transform().set_rotation(rotation);
            InspectorPanel::node_selection->get_transform().set_scale(scale);
        }
    }

    ImGui::End();

    ImGui::End();
    ImGui::PopStyleVar();

    ImGuiLayer::end_frame();
}

void Editor::on_destroy()
{
    m_current_scene->on_destroy();
    ImGuiLayer::destroy();
}

void Editor::on_event(Event& e)
{
    if (e.get_type() == EventType::KeyPress)
    {
        if (!m_is_playing)
        {
            
            KeyPressEvent& ke = static_cast<KeyPressEvent&>(e);
            if (ke.get_key() == Key::Q)
                m_imguizmo_operation = ImGuizmo::OPERATION::TRANSLATE;
            else if (ke.get_key() == Key::W)
                m_imguizmo_operation = ImGuizmo::OPERATION::SCALE;
            else if (ke.get_key() == Key::E)
                m_imguizmo_operation = ImGuizmo::OPERATION::ROTATE;

        }
    }

    if (e.get_type() != EventType::WindowResize || m_is_playing) // Viewport is different to window with ImGui
    {
        m_current_scene->on_event(e);
    }
}

void Editor::render_menu_bar()
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
            {
                Serializer::serialize_scene(m_current_scene, "assets/test.scene");
            }

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
}

void Editor::open_scene(const std::string& path)
{
    m_current_scene = Deserializer::deserialize_scene(path);

    ScenePanel::set_scene(m_current_scene);
    InspectorPanel::set_scene(m_current_scene);

    InspectorPanel::node_selection = nullptr;
}