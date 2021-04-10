#include "engine/renderer/transform.h"
#include "engine/scene/node.h"
#include "engine/maths/quaternion.h"

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

Matrix4f Transform::get_world_transform() const
{
    Matrix4f local = Matrix4f::translate(Matrix4f(1.0f), m_translation)
                   * Quaternionf::to_matrix4(Quaternionf(m_rotation))
                   * Matrix4f::scale(Matrix4f(1.0f), m_scale);

    if (!m_owner->get_parent())
    {
        return local;
    }

    Matrix4f world = m_owner->get_parent()->get_transform().get_world_transform();

    return world * local;
}

Matrix4f Transform::get_local_transform() const
{
    return Matrix4f::translate(Matrix4f(1.0f), m_translation)
         * Quaternionf::to_matrix4(Quaternionf(m_rotation))
         * Matrix4f::scale(Matrix4f(1.0f), m_scale);
}