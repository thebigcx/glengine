#include "engine/scene/scene.h"
#include "engine/scene/node.h"
#include "engine/renderer/renderer_2d.h"
#include "engine/renderer/camera.h"
#include "editor/editor_camera.h"

#include <algorithm>

Scene::Scene()
{
    m_root_node = new Node("Root", nullptr, this);
}

Scene::~Scene()
{
    delete m_root_node;
}

void Scene::on_render()
{
    if (!Camera::main_camera)
        return;

    Renderer2D::start_scene(Camera::main_camera->get_view_matrix(), Camera::main_camera->get_projection_matrix());
    
    m_root_node->on_render();

    Renderer2D::flush_batch();
}

void Scene::on_editor_render(const EditorCamera& camera)
{
    Renderer2D::start_scene(camera.get_view_matrix(), camera.get_projection_matrix());
    
    m_root_node->on_render();

    Renderer2D::flush_batch();
}

void Scene::on_update(float dt)
{
    m_root_node->on_update(dt);
}

void Scene::on_editor_update(float dt)
{
    /*for (auto& node : m_nodes)
    {
        node->on_update(dt);
    }*/
}

Node* Scene::create_node(const std::string& name)
{
    return m_root_node->create_child(name);
}

void Scene::remove_node(Node* node)
{
    m_root_node->remove_child(node);
}

void Scene::on_event(Event& e)
{
    m_root_node->on_event(e);
}

void Scene::on_start()
{
    m_root_node->on_start();
}

void Scene::on_destroy()
{
    m_root_node->on_destroy();
}