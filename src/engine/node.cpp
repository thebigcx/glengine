#include "engine/node.h"

Node::Node(const std::string& name, Node* parent, Scene* scene)
    : m_name(name), m_parent(parent), m_scene(scene)
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