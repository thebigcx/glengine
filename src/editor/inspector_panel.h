#pragma once

#include <memory>

class Scene;
class Node;

class InspectorPanel
{
public:
    static void set_scene(const std::shared_ptr<Scene>& scene);
    static void imgui_render();

private:
    static inline std::weak_ptr<Scene> m_scene_context;
};