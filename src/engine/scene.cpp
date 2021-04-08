#include "engine/scene.h"
#include "engine/node.h"

Scene::Scene()
{

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