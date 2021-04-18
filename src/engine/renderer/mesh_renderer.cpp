#include "engine/renderer/mesh_renderer.h"
#include "engine/scene/node.h"
#include "engine/renderer/mesh.h"
#include "engine/renderer/renderer_3d.h"
#include "engine/renderer/material.h"
#include "engine/core/serializer.h"

#include <glad/glad.h>

void MeshRenderer::on_render()
{
    if (!m_owner->has_component<Mesh>())
        return; // No mesh to render

    if (!m_material.lock())
        return;

    if (!m_material.lock()->get_shader().lock())
        return;

    glEnable(GL_DEPTH_TEST);

    Renderer3D::render_mesh(*m_owner->get_component<Mesh>(), m_owner->get_transform().get_world_transform(), m_material.lock());
}

void MeshRenderer::serialize(YAML::Node& node)
{
    Serializer::serialize_mesh_renderer(node, this);
}