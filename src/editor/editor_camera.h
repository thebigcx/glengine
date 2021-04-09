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

    Vector3f get_right() const;
    Vector3f get_forward() const;
    Vector3f get_up() const;

    void set_focus(bool focus) { m_has_focus = focus; }

private:
    float m_fov = 45, m_aspect, m_near_clip = 0.1f, m_far_clip = 100.0f;

    float m_pitch = 0, m_yaw = 0;

    float m_viewport_width, m_viewport_height;

    Matrix4f m_view_matrix, m_projection_matrix;

    Vector3f m_position = { 0, 0, 1 };

    bool m_mouse_dragging = false;
    Vector2i m_mouse_last_position;
    
    bool m_is_first_frame = true;

    float m_speed = 0.1f;
    float m_sensitivity = 0.3f;

    bool m_has_focus = true;
};