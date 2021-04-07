#pragma once

class Node;

class Component
{
public:
    virtual ~Component() {}

    Node* get_owner() const { return m_owner; }

protected:
    Node* m_owner;
};