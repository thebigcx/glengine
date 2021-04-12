#pragma once

#include "engine/scene/component.h"
#include "engine/maths/matrix4.h"

enum class CameraProjection
{
    Orthographic,
    Perspective
};

class Camera : public Component
{
private:
    struct OrthoData
    {
        float size = 10.0f, near_clip = -1, far_clip = 1;
    };

    struct PerspectiveData
    {
        float fov = 45, near_clip = 0.1f, far_clip = 100.0f;
    };

public:
    Camera();

    void on_start() override;

    void on_event(Event& e) override;
    void on_transform_change() override;

    void on_window_resize(uint32_t width, uint32_t height);

    void set_ortho(float size, float near_clip, float far_clip);
    void set_perspective(float fov, float near_clip, float far_clip);

    CameraProjection get_projection_type() const { return m_projection_type; }
    void set_projection_type(CameraProjection type);

    void calculate_projection();

    const Matrix4f& get_view_matrix() const { return m_view_matrix; }
    const Matrix4f& get_projection_matrix() const { return m_projection_matrix; }

    static inline Camera* main_camera = nullptr;

    static void set_main_camera(Camera* camera);
    bool is_main_camera() const { return m_is_main_camera; }

    const OrthoData& get_ortho() const { return m_ortho_data; }
    const PerspectiveData& get_perspective() const { return m_perspective_data; }

    void serialize(YAML::Node& node) override;

private:
    Matrix4f m_view_matrix, m_projection_matrix;

    float m_aspect;

    CameraProjection m_projection_type = CameraProjection::Orthographic;

    OrthoData m_ortho_data;
    PerspectiveData m_perspective_data;

    bool m_is_main_camera = false;

};