#include "engine/camera.h"
#include "engine/event.h"
#include "engine/node.h"
#include "engine/maths/vector3.h"
#include "engine/maths/quaternion.h"

#include <iostream>

Camera::Camera()
{

}

void Camera::on_event(Event& e)
{
    if (e.get_type() == EventType::WindowResize)
    {
        WindowResizeEvent& window_event = static_cast<WindowResizeEvent&>(e);
        on_window_resize(window_event.get_width(), window_event.get_height());
    }
}

void Camera::on_transform_change()
{
    Quaternionf q(m_owner->get_transform().get_rotation());

    Vector3f pos = m_owner->get_transform().get_translation();
    Vector3f direction = Quaternionf::rotate(q, Vector3f(0, 0, -1)); // Multiple rotation with a foward vector
    Vector3f up  = Quaternionf::rotate(q, Vector3f(0, 1, 0)); // Multiple rotation with an up vector

    m_view_matrix = Matrix4f::look_at(pos, direction, up);
}

void Camera::on_window_resize(uint32_t width, uint32_t height)
{
    m_aspect = static_cast<float>(width) / static_cast<float>(height);
    calculate_projection();
}

void Camera::set_ortho(float size, float near_clip, float far_clip)
{
    m_projection_type = CameraProjection::Orthographic;

    m_ortho_data.size = size;
    m_ortho_data.near_clip = near_clip;
    m_ortho_data.far_clip = far_clip;

    calculate_projection();
}

void Camera::set_perspective(float fov, float near_clip, float far_clip)
{
    m_projection_type = CameraProjection::Perspective;

    m_perspective_data.fov = fov;
    m_perspective_data.near_clip = near_clip;
    m_perspective_data.far_clip = far_clip;

    calculate_projection();
}

void Camera::calculate_projection()
{
    if (m_projection_type == CameraProjection::Perspective)
    {
        m_projection_matrix = Matrix4f::perspective(m_perspective_data.fov, m_aspect, m_perspective_data.near_clip, m_perspective_data.far_clip);
    }
    else
    {
        float left = -m_ortho_data.size * m_aspect * 0.5f;
        float right = m_ortho_data.size * m_aspect * 0.5f;
        float bottom = -m_ortho_data.size * 0.5f;
        float top = m_ortho_data.size * 0.5f;

        m_projection_matrix = Matrix4f::ortho(left, right, bottom, top, m_ortho_data.near_clip, m_ortho_data.far_clip);
    }
}