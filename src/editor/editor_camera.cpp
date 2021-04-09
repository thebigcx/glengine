#include "editor/editor_camera.h"
#include "engine/maths/math.h"
#include "engine/keyboard.h"
#include "engine/mouse.h"

EditorCamera::EditorCamera()
{
    
}

void EditorCamera::on_update(float dt)
{
    if (m_has_focus)
    {
        if (Mouse::is_mouse_pressed(MouseButton::Left))
        {
            Vector2i mouse_current_position = Mouse::get_mouse_position();

            if (m_is_first_frame)
            {
                m_mouse_last_position = mouse_current_position;
                m_is_first_frame = false;
            }

            Vector2i off = mouse_current_position - m_mouse_last_position;
            m_yaw += off.x * m_sensitivity;
            m_pitch += off.y * m_sensitivity;

            m_mouse_last_position = mouse_current_position;
        }
        else
        {
            m_is_first_frame = true;
        }

        if (Keyboard::is_key_pressed(Key::A))
        {
            m_position -= get_right() * m_speed;
        }
        if (Keyboard::is_key_pressed(Key::D))
        {
            m_position += get_right() * m_speed;
        }
        if (Keyboard::is_key_pressed(Key::W))
        {
            m_position.x += get_forward().x * m_speed;
            m_position.z += get_forward().z * m_speed;
        }
        if (Keyboard::is_key_pressed(Key::S))
        {
            m_position.x -= get_forward().x * m_speed;
            m_position.z -= get_forward().z * m_speed;
        }
        if (Keyboard::is_key_pressed(Key::LeftShift))
        {
            m_position.y -= m_speed;
        }
        if (Keyboard::is_key_pressed(Key::Space))
        {
            m_position.y += m_speed;
        }
    }
    else
    {
        m_is_first_frame = true;
    }

    calculate_view();
}

void EditorCamera::calculate_projection()
{
    m_aspect = static_cast<float>(m_viewport_width) / static_cast<float>(m_viewport_height);
    m_projection_matrix = Matrix4f::perspective(static_cast<float>(Math::to_radians(m_fov)), m_aspect, m_near_clip, m_far_clip);
}

void EditorCamera::calculate_view()
{
    m_view_matrix = Matrix4f::look_at(m_position, m_position + get_forward(), get_up());
}

void EditorCamera::on_viewport_resize(uint32_t width, uint32_t height)
{
    m_viewport_width = width;
    m_viewport_height = height;
    calculate_projection();
}

Quaternionf EditorCamera::get_orientation() const
{
    return Quaternionf(Vector3f(Math::to_radians(-m_pitch), Math::to_radians(-m_yaw), 0.0f));
}

Vector3f EditorCamera::get_right() const
{
    return Quaternionf::rotate(get_orientation(), Vector3f(1, 0, 0));
}

Vector3f EditorCamera::get_forward() const
{
    return Quaternionf::rotate(get_orientation(), Vector3f(0, 0, -1));
}

Vector3f EditorCamera::get_up() const
{
    return Quaternionf::rotate(get_orientation(), Vector3f(0, 1, 0));
}