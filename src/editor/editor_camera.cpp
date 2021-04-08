#include "editor/editor_camera.h"
#include "engine/maths/math.h"

#include <iostream>

EditorCamera::EditorCamera()
{
    calculate_projection();
}

void EditorCamera::on_update(float dt)
{
    

    calculate_view();
}

void EditorCamera::calculate_projection()
{
    m_aspect = static_cast<float>(m_viewport_width) / static_cast<float>(m_viewport_height);
    m_projection_matrix = Matrix4f::perspective(static_cast<float>(Math::to_radians(m_fov)), m_aspect, m_near_clip, m_far_clip);
    //m_projection_matrix = Matrix4f::ortho(0, 1280, 0, 720, -1000, 1000);
}

void EditorCamera::calculate_view()
{
    m_view_matrix = Matrix4f::look_at(m_position, m_position + m_forward, m_up);

    /*Quaternionf orient = get_orientation();
    m_view_matrix = Matrix4f::translate(Matrix4f(1.0f), m_position) * Quaternionf::to_matrix4(orient);
    m_view_matrix = Matrix4f::inverse(m_view_matrix);*/
}

void EditorCamera::on_viewport_resize(uint32_t width, uint32_t height)
{
    m_viewport_width = width;
    m_viewport_height = height;
    calculate_projection();
}

Quaternionf EditorCamera::get_orientation() const
{
    return Quaternionf(Vector3f(-m_pitch, -m_yaw, 0.0f));
}