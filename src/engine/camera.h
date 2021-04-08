#pragma once

#include "engine/component.h"
#include "engine/maths/matrix4.h"

enum class CameraProjection
{
    Orthographic,
    Perspective
};

class Camera : public Component
{
public:
    Camera();

    void on_event(Event& e) override;
    void on_transform_change() override;

    void on_window_resize(uint32_t width, uint32_t height);

    void set_ortho(float size, float near_clip, float far_clip);
    void set_perspective(float fov, float near_clip, float far_clip);

    void calculate_projection();

    const Matrix4f& get_view_matrix() const { return m_view_matrix; }
    const Matrix4f& get_projection_matrix() const { return m_projection_matrix; }

private:
    Matrix4f m_view_matrix, m_projection_matrix;

    float m_aspect;

    CameraProjection m_projection_type;

    struct
    {
        float size = 1000, near_clip = -1, far_clip = 1;
    } m_ortho_data;

    struct
    {
        float fov, near_clip, far_clip;
    } m_perspective_data;

};