#pragma once

#include <memory>

#include "engine/maths/matrix4.h"

class Camera;
class Mesh;
class Shader;
class Material;

class Renderer3D
{
private:
    static inline struct Renderer3DData
    {
        Matrix4f projection;
        Matrix4f view;
    } m_data;

public:
    static void init();

    static void start_scene(const Matrix4f& view, const Matrix4f& projection);
    static void render_mesh(const Mesh& mesh, const Matrix4f& transform, const std::shared_ptr<Material>& material);
    static void end_scene();
};