#include "editor/editor.h"
#include "editor/imgui_layer.h"
#include "engine/scene/scene.h"
#include "engine/scene/node.h"
#include "engine/renderer/renderer_2d.h"
#include "engine/renderer/camera.h"
#include "engine/core/event.h"
#include "engine/renderer/framebuffer.h"
#include "engine/renderer/assets.h"
#include "engine/core/serializer.h"
#include "engine/core/deserializer.h"

#include "editor/panels/scene_panel.h"
#include "editor/panels/inspector_panel.h"
#include "editor/panels/assets_panel.h"
#include "editor/panels/debug_panel.h"

#include "imgui/imgui.h"
#include "editor/imguizmo/ImGuizmo.h"
#include "editor/fork_awesome/fork_awesome_icons.h"

#include <iostream>

#include <glad/glad.h>

Editor::Editor()
{
    
}

void Editor::on_start()
{
    Scene::current_scene = AssetManager::get_instance()->get_scene("new");

    ImGuiLayer::init();
    ScenePanel::set_scene(Scene::current_scene.lock());
    InspectorPanel::set_scene(Scene::current_scene.lock());

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

        Scene::current_scene.lock()->on_update(dt);
        Scene::current_scene.lock()->on_render();
        
        if (Keyboard::is_key_pressed(Key::Escape))
        {
            m_is_playing = false;
            Scene::current_scene.lock()->on_destroy();

            Scene::current_scene = AssetManager::get_instance()->get_scene("temp"); // TODO: serialize in string rather than file

            ScenePanel::set_scene(Scene::current_scene.lock());
            InspectorPanel::set_scene(Scene::current_scene.lock());

            InspectorPanel::node_selection = nullptr;
        }

        return;
    }

    Scene::current_scene.lock()->on_editor_update(dt);
    m_camera.on_update(dt);

    m_framebuffer->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Scene::current_scene.lock()->on_editor_render(m_camera);

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
    DebugPanel::imgui_render();

    ImGui::Begin("ScenePlay");
    if (ImGui::Button(ICON_FK_PLAY))
    {
        m_is_playing = true;
        Serializer::serialize_scene(Scene::current_scene.lock(), "assets/temp.scene");
        Scene::current_scene.lock()->on_start();
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
        Scene::current_scene.lock()->on_event(e);
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

    if (e.get_type() == EventType::ScriptError)
    {
        DebugPanel::write_msg(static_cast<ScriptErrorEvent&>(e).get_msg() + "\n");
    }
    
    if (e.get_type() != EventType::WindowResize || m_is_playing) // Viewport is different to window with ImGui
    {
        Scene::current_scene.lock()->on_event(e);
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
                Serializer::serialize_scene(Scene::current_scene.lock(), "assets/test.scene");
            }
            if (ImGui::MenuItem("Exit", "Ctrl+Q"))
            {
                Application::get_instance()->quit();
            }

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
}

void Editor::open_scene(const std::string& name)
{
    Scene::current_scene = AssetManager::get_instance()->get_scene(name);
    //Scene::current_scene = Deserializer::deserialize_scene("assets/" + name + ".scene");

    ScenePanel::set_scene(Scene::current_scene.lock());
    InspectorPanel::set_scene(Scene::current_scene.lock());

    InspectorPanel::node_selection = nullptr;
}
