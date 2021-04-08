#pragma once

#include "engine/maths/vector3.h"

class Node;

class Transform
{
public:
    Transform(Node* owner);

    void set_translation(const Vector3f& translation);
    void set_rotation(const Vector3f& rotation);
    void set_scale(const Vector3f& scale);

    const Vector3f& get_translation() const { return m_translation; }
    const Vector3f& get_rotation() const { return m_rotation; }
    const Vector3f& get_scale() const { return m_scale; }

private:
    Vector3f m_translation, m_rotation, m_scale;

    Node* m_owner;
};