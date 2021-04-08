#include "engine/node.h"
#include "engine/component.h"

Node::Node(const std::string& name, Node* parent, Scene* scene)
    : m_name(name), m_parent(parent), m_scene(scene), m_transform(this)
{

}

Node::~Node()
{
    for (auto& child : m_children)
    {
        delete child;
    }
}

Node* Node::create_child(const std::string& name)
{
    Node* node = new Node(name, this, m_scene);
    m_children.push_back(node);
    return node;
}

void Node::set_transform_change()
{
    for (auto& c : m_components)
    {
        c.second->on_transform_change();
    }

    for (auto& c : m_children)
    {
        c->set_transform_change();
    }
}

void Node::on_event(Event& e)
{
    for (auto& c : m_components)
    {
        c.second->on_event(e);
    }

    for (auto& c : m_children)
    {
        c->on_event(e);
    }
}