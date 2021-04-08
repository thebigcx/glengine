#pragma once

class Node;
class Event;

class Component
{
    friend class Node;

public:
    virtual ~Component() {}

    Node* get_owner() const { return m_owner; }

    virtual void on_update(float dt) {}
    virtual void on_event(Event& e) {}
    virtual void on_transform_change() {}

protected:
    Node* m_owner;
};