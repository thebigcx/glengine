#include "engine/renderer/renderer_3d.h"
#include "engine/renderer/shader.h"
#include "engine/renderer/mesh.h"
#include "engine/renderer/material.h"
#include "engine/renderer/texture.h"

#include <glad/glad.h>

void Renderer3D::init()
{

}

void Renderer3D::start_scene(const Matrix4f& view, const Matrix4f& projection)
{
    m_data.projection = projection;
    m_data.view = view;
}

void Renderer3D::render_mesh(const Mesh& mesh, const Matrix4f& transform, const std::shared_ptr<Material>& material)
{
    std::shared_ptr<Shader> shader = material->get_shader().lock();

    shader->bind();
    shader->uniform_matrix4("u_projection", m_data.projection);
    shader->uniform_matrix4("u_view", m_data.view);
    shader->uniform_matrix4("u_transform", transform);

    material->get_albedo().lock()->bind(0);

    glBindVertexArray(mesh.get_vertex_array());

    glDrawElements(GL_TRIANGLES, mesh.get_index_count(), GL_UNSIGNED_INT, 0);
}

void Renderer3D::end_scene()
{

}