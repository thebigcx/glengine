#pragma once

#include "engine/maths/matrix4.h"
#include "engine/maths/quaternion.h"

class EditorCamera
{
public:
    EditorCamera();

    void on_update(float dt);
    void on_viewport_resize(uint32_t width, uint32_t height);

    const Matrix4f& get_view_matrix() const { return m_view_matrix; }
    const Matrix4f& get_projection_matrix() const { return m_projection_matrix; }

    void calculate_projection();
    void calculate_view();

    Quaternionf get_orientation() const;

private:
    float m_fov = 45, m_aspect, m_near_clip = 0.1f, m_far_clip = 100.0f;

    float m_pitch = 0, m_yaw = 0;

    float m_viewport_width, m_viewport_height;

    Vector3f m_position = { 0, 0, 1 };
    Vector3f m_forward = { 0, 0, -1 };
    Vector3f m_up = { 0, 1, 0 };

    Matrix4f m_view_matrix, m_projection_matrix;
};