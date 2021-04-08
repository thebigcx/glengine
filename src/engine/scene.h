#pragma once

#include <vector>
#include <string>

class Node;
class Event;

class Scene
{
public:
    Scene();

    Node* create_node(const std::string& name);

    const std::vector<Node*>& get_nodes() const { return m_nodes; }

    void on_event(Event& e);

private:
    std::vector<Node*> m_nodes;
};