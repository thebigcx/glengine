#pragma once

#include <memory>

class Scene;
class Node;

class ScenePanel
{
public:
    static void set_scene(const std::shared_ptr<Scene>& scene);
    static void imgui_render();

    static Node* get_selected_node() { return m_selected_node; }

private:
    static inline std::weak_ptr<Scene> m_scene_context;
    static inline Node* m_selected_node = nullptr;

    static void render_node(Node* node);
};