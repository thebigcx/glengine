#pragma once

#include <vector>
#include <string>
#include <memory>

class Node;
class Event;
class EditorCamera;

// TODO: Scene should inherit from node, a root node. Would make children and nodes much easier.
class Scene
{
public:
    Scene();
    ~Scene();

    void on_start();
    void on_render();
    void on_editor_render(const EditorCamera& camera);
    void on_update(float dt);
    void on_editor_update(float dt);
    void on_destroy();

    Node* create_node(const std::string& name);
    void remove_node(Node* node);

    Node* get_root_node() const { return m_root_node; }

    void on_event(Event& e);

    static inline std::weak_ptr<Scene> current_scene;

private:
    Node* m_root_node;
};