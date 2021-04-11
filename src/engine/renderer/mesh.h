#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "engine/scene/component.h"
#include "engine/maths/vector2.h"
#include "engine/maths/vector3.h"

struct Vertex3D
{
    Vector3f pos;
    Vector3f normal;
    Vector3f tangent;
    Vector2f uv;
};

struct MeshData
{
    std::vector<Vertex3D> vertices;
    std::vector<uint32_t> indices;
};

class Mesh : public Component
{
public:
    Mesh();
    ~Mesh();

    void load(const std::string& path);

    uint32_t get_vertex_array() const { return m_vao; }
    uint32_t get_index_count() const { return m_index_count; }

    const std::string& get_path() const { return m_path; }

private:
    uint32_t m_vao = 0, m_vbo = 0, m_ebo = 0;
    uint32_t m_index_count = 0;

    std::string m_path = "";

    
};

class AssimpLoader
{
public:
    static MeshData load_mesh_from_file(const std::string& path);
};