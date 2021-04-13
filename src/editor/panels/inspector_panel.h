#pragma once

#include <memory>
#include <string>

enum class SelectionType
{
    Node, Material, Texture
};

class Scene;
class Node;

class InspectorPanel
{
public:
    static void set_scene(Scene* scene);
    static void imgui_render();

    static void render_node_inspector();
    static void render_material_inspector();
    static void render_texture_inspector();

    static inline Node* node_selection;
    static inline std::string material_selection;
    static inline std::string texture_selection;
    static inline SelectionType selection_type;

private:
    static inline Scene* m_scene_context;

    template<typename T>
    static void add_component(Node* node, const std::string& name); // Add component menu item

    template<typename T, typename F>
    static void draw_component(const std::string& name, Node* node, const F& fn);

    static void draw_components(Node* node);
};