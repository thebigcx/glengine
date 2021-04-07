#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <typeindex>

class Scene;
class Component;

class Node
{
public:
    Node(const std::string& name, Node* parent, Scene* scene);
    ~Node();

    Node* create_child(const std::string& name);

    const std::string& get_name() const { return m_name; }
    Node* get_parent() const { return m_parent; }
    Scene* get_scene() const { return m_scene; }
    const std::vector<Node*>& get_children() const { return m_children; }

    template<typename T, typename... Args>
    Component* create_component(Args&&... args)
    {
        Component* component = new T(std::forward<Args>(args)...);
        m_components.emplace(typeid(T), component);
        return component;
    }

    template<typename T>
    bool has_component()
    {
        return m_components.find(typeid(T)) != m_components.end();
    }

private:
    std::string m_name;

    Node* m_parent;
    Scene* m_scene;

    std::vector<Node*> m_children;
    std::unordered_map<std::type_index, Component*> m_components;
};