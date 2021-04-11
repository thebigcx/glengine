#include "engine/renderer/mesh.h"
#include "engine/renderer/renderer_3d.h"
#include "engine/scene/node.h"
#include "engine/renderer/assets.h"

#include <glad/glad.h>
#include <yaml-cpp/yaml.h>

Mesh::Mesh()
{

}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
}

void Mesh::load(const std::string& path)
{
    m_path = path;

    MeshData data = AssimpLoader::load_mesh_from_file(path);

    m_index_count = data.indices.size();

    glCreateVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glCreateBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, data.vertices.size() * sizeof(Vertex3D), data.vertices.data(), GL_STATIC_DRAW);

    glCreateBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.indices.size() * sizeof(uint32_t), data.indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, pos));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, tangent));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, uv));
    glEnableVertexAttribArray(3);
}

void Mesh::serialize(YAML::Node& node)
{
    node["Mesh"]["Path"] = m_path;
}