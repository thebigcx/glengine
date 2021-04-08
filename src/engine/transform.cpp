#include "engine/transform.h"
#include "engine/node.h"

Transform::Transform(Node* owner)
    : m_owner(owner), m_scale(1.0f), m_rotation(0)
{
    
}

void Transform::set_translation(const Vector3f& translation)
{
    if (m_translation != translation)
    {
        m_translation = translation;
        m_owner->set_transform_change();
    }
}

void Transform::set_rotation(const Vector3f& rotation)
{
    if (m_rotation != rotation)
    {
        m_rotation = rotation;
        m_owner->set_transform_change();
    }
}

void Transform::set_scale(const Vector3f& scale)
{
    if (m_scale != scale)
    {
        m_scale = scale;
        m_owner->set_transform_change();
    }
}