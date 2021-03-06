#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <typeindex>

#include "engine/renderer/transform.h"
#include "engine/scene/component.h"

class Scene;
class Event;

class Node
{
    friend class Transform;

public:
    Node(const std::string& name, Node* parent, Scene* scene);
    ~Node();

    Node* create_child(const std::string& name);
    void remove_child(Node* child);

    const std::string& get_name() const { return m_name; }
    Node* get_parent() const { return m_parent; }
    Scene* get_scene() const { return m_scene; }
    const std::vector<Node*>& get_children() const { return m_children; }

    // Create a component at runtime
    template<typename T, typename... Args>
    T* create_component(Args&&... args)
    {
        Component* component = new T(std::forward<Args>(args)...);
        component->m_owner = this;
        m_components.emplace(typeid(T), component);
        return static_cast<T*>(component);
    }

    template<typename T>
    T* get_component()
    {
        if (!has_component<T>())
        {
            printf("Node does not contain component.\n");
            return nullptr;
        }

        return static_cast<T*>(m_components.at(typeid(T)));
    }

    Transform& get_transform() { return m_transform; }

    template<typename T>
    bool has_component()
    {
        return m_components.find(typeid(T)) != m_components.end();
    }

    template<typename T>
    void remove_component()
    {
        if (!has_component<T>())
            return;

        delete m_components[typeid(T)];
        m_components.erase(typeid(T));
    }

    void on_start();
    void on_render();
    void on_update(float dt);
    void on_event(Event& e);
    void on_destroy();

    void set_name(const std::string& name) { m_name = name; }

    void set_enabled(bool enabled = true) { m_enabled = enabled; }
    bool is_enabled() const { return m_enabled; }

    Node* find_child(const std::string& name);

    const std::unordered_map<std::type_index, Component*>& get_components() const { return m_components; }

private:
    std::string m_name;
    bool m_enabled = true;

    Node* m_parent;
    Scene* m_scene;

    std::vector<Node*> m_children;
    std::unordered_map<std::type_index, Component*> m_components;

    Transform m_transform;

    // Visible to transform class (friend)
    void set_transform_change();
};