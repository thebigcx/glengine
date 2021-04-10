#include "engine/node.h"
#include "engine/component.h"

#include <algorithm>

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
    if (!m_enabled)
        return;

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
    if (!m_enabled)
        return;

    for (auto& c : m_components)
    {
        c.second->on_event(e);
    }

    for (auto& c : m_children)
    {
        c->on_event(e);
    }
}

void Node::on_render()
{
    if (!m_enabled)
        return;

    for (auto& c : m_components)
    {
        c.second->on_render();
    }

    for (auto& c : m_children)
    {
        c->on_render();
    }
}

void Node::on_update(float dt)
{
    if (!m_enabled)
        return;

    for (auto& c : m_components)
    {
        c.second->on_update(dt);
    }

    for (auto& c : m_children)
    {
        c->on_update(dt);
    }
}

void Node::remove_child(Node* child)
{
    m_children.erase(std::find(m_children.begin(), m_children.end(), child));
}

void Node::on_start()
{
    if (!m_enabled)
        return;

    for (auto& c : m_components)
        c.second->on_start();

    for (auto& c : m_children)
        c->on_start();
}

void Node::on_destroy()
{
    if (!m_enabled)
        return;

    for (auto& c : m_components)
        c.second->on_destroy();

    for (auto& c : m_children)
        c->on_destroy();
}