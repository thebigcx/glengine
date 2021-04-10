#pragma once

#include <vector>
#include <string>

class Node;
class Event;
class EditorCamera;

// TODO: Scene should inherit from node, a root node. Would make children and nodes much easier.
class Scene
{
public:
    Scene();

    void on_start();
    void on_render();
    void on_editor_render(const EditorCamera& camera);
    void on_update(float dt);
    void on_editor_update(float dt);
    void on_destroy();

    Node* create_node(const std::string& name);
    void remove_node(Node* node);

    const std::vector<Node*>& get_nodes() const { return m_nodes; }

    void on_event(Event& e);

private:
    std::vector<Node*> m_nodes;
};