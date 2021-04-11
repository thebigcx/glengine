#pragma once

class Node;
class Event;

namespace YAML
{
    class Node;
}

class Component
{
    friend class Node;

public:
    virtual ~Component() {}

    Node* get_owner() const { return m_owner; }

    virtual void on_start() {}
    virtual void on_render() {} // Update and Render are separate to allow multithreading
    virtual void on_update(float dt) {}
    virtual void on_destroy() {}
    virtual void on_event(Event& e) {}
    virtual void on_transform_change() {}

    virtual void serialize(YAML::Node& node) = 0;

protected:
    Node* m_owner;
};