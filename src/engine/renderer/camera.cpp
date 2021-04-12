#include "engine/renderer/camera.h"
#include "engine/core/event.h"
#include "engine/scene/node.h"
#include "engine/maths/vector3.h"
#include "engine/maths/quaternion.h"
#include "engine/maths/math.h"
#include "engine/core/application.h"

#include <yaml-cpp/yaml.h>
#include <iostream>

Camera::Camera()
{

}

void Camera::on_start()
{
    on_transform_change();
    on_window_resize(Application::get_instance()->get_window()->get_width(), Application::get_instance()->get_window()->get_height());
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
    Vector3f rotation = m_owner->get_transform().get_rotation();
    Quaternionf q(Vector3f(Math::to_radians(rotation.x), Math::to_radians(rotation.y), Math::to_radians(rotation.z)));

    Vector3f pos = m_owner->get_transform().get_translation();
    Vector3f direction = pos + Quaternionf::rotate(q, Vector3f(0, 0, -1)); // Multiple rotation with a foward vector
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

void Camera::set_projection_type(CameraProjection type)
{
    m_projection_type = type;
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

void Camera::set_main_camera(Camera* camera)
{
    if (main_camera)
    {
        main_camera->m_is_main_camera = false;
    }
    
    main_camera = camera;

    if (camera) // Set to nullptr for no camera
        camera->m_is_main_camera = true;
}

void Camera::serialize(YAML::Node& node)
{
    node["Camera"]["Main"] = m_is_main_camera;

    if (m_projection_type == CameraProjection::Orthographic)
        node["Camera"]["Projection Type"] = "Orthographic";
    else
        node["Camera"]["Projection Type"] = "Perspective";

    node["Camera"]["Orthographic"]["Size"] = m_ortho_data.size;
    node["Camera"]["Orthographic"]["Near Clip"] = m_ortho_data.near_clip;
    node["Camera"]["Orthographic"]["Far Clip"] = m_ortho_data.far_clip;

    node["Camera"]["Perspective"]["FOV"] = m_perspective_data.fov;
    node["Camera"]["Perspective"]["Near Clip"] = m_perspective_data.near_clip;
    node["Camera"]["Perspective"]["Far Clip"] = m_perspective_data.far_clip;
}