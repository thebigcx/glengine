#include "engine/scene.h"
#include "engine/node.h"
#include "engine/renderer_2d.h"
#include "engine/camera.h"
#include "editor/editor_camera.h"

Scene::Scene()
{

}

void Scene::on_render()
{
    if (!Camera::main_camera)
        return;

    Renderer2D::start_scene(Camera::main_camera->get_view_matrix(), Camera::main_camera->get_projection_matrix());
    
    for (auto& node : m_nodes)
    {
        node->on_render();
    }

    Renderer2D::flush_batch();
}

void Scene::on_editor_render(const EditorCamera& camera)
{
    Renderer2D::start_scene(camera.get_view_matrix(), camera.get_projection_matrix());
    
    for (auto& node : m_nodes)
    {
        node->on_render();
    }

    Renderer2D::flush_batch();
}

void Scene::on_update(float dt)
{
    for (auto& node : m_nodes)
    {
        node->on_update(dt);
    }
}

Node* Scene::create_node(const std::string& name)
{
    Node* node = new Node(name, nullptr, this);
    m_nodes.emplace_back(node);
    return node;
}

void Scene::on_event(Event& e)
{
    for (auto& node : m_nodes)
    {
        node->on_event(e);
    }
}