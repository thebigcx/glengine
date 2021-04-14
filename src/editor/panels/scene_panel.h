#pragma once

#include <memory>

class Scene;
class Node;

class ScenePanel
{
public:
    static void set_scene(const std::shared_ptr<Scene>& scene);
    static void imgui_render();

private:
    static inline std::shared_ptr<Scene> m_scene_context;

    static void render_node(Node* node);

    static inline Node* m_node_to_delete;
};