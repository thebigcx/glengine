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

    template<typename T>
    static void add_component(Node* node, const std::string& name); // Add component menu item

    template<typename T, typename F>
    static void draw_component(const std::string& name, Node* node, const F& fn);

    static void draw_components(Node* node);
};