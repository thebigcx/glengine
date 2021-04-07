#pragma once

#include <vector>
#include <string>

class Node;

class Scene
{
public:
    Scene();

    Node* create_node(const std::string& name);

    const std::vector<Node*>& get_nodes() const { return m_nodes; }

private:
    std::vector<Node*> m_nodes;
};