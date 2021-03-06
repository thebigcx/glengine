#include "engine/renderer/renderer_2d.h"
#include "engine/renderer/texture.h"
#include "engine/renderer/assets.h"
#include "engine/renderer/shader.h"
#include "engine/renderer/camera.h"
#include "engine/maths/quaternion.h"

#include <glad/glad.h>

void Renderer2D::init()
{
    m_data.vertex_base = new SpriteVertex[MAX_SPRITES];
    m_data.vertex_ptr = m_data.vertex_base;

    glCreateVertexArrays(1, &m_data.vao);
    glBindVertexArray(m_data.vao);

    glCreateBuffers(1, &m_data.vbo);
    glCreateBuffers(1, &m_data.ebo);

    glBindBuffer(GL_ARRAY_BUFFER, m_data.vbo);
    glBufferData(GL_ARRAY_BUFFER, MAX_VERTICES * sizeof(SpriteVertex), nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), reinterpret_cast<void*>(offsetof(SpriteVertex, pos)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), reinterpret_cast<void*>(offsetof(SpriteVertex, uv)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), reinterpret_cast<void*>(offsetof(SpriteVertex, color)));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), reinterpret_cast<void*>(offsetof(SpriteVertex, index)));
    glEnableVertexAttribArray(3);

    uint32_t indices[MAX_INDICES];

    uint32_t offset = 0;
    for (uint32_t i = 0; i < MAX_INDICES; i += 6, offset += 4)
    {
        indices[i + 0] = 0 + offset;
        indices[i + 1] = 1 + offset;
        indices[i + 2] = 2 + offset;

        indices[i + 3] = 2 + offset;
        indices[i + 4] = 3 + offset;
        indices[i + 5] = 0 + offset;
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_data.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, MAX_INDICES * sizeof(uint32_t), indices, GL_STATIC_DRAW);

    m_data.shader = AssetManager::get_instance()->get_shader("assets/texture_2d.glsl");

    uint32_t white = 0xffffffff;
    m_data.textures[0] = std::make_shared<Texture>(1, 1, &white);

    int32_t samplers[32];
    for (uint32_t i = 0; i < 32; i++)
        samplers[i] = i;

    m_data.shader.lock()->bind();
    glUniform1iv(m_data.shader.lock()->get_uniform_location("u_textures"), 32, samplers);
}

void Renderer2D::finalize()
{
    delete m_data.vertex_base;

    glDeleteVertexArrays(1, &m_data.vao);
    glDeleteBuffers(1, &m_data.vbo);
    glDeleteBuffers(1, &m_data.ebo);
}

void Renderer2D::start_batch()
{
    m_data.vertex_ptr = m_data.vertex_base;
    m_data.vertex_count = 0;
    m_data.texture_slot_index = 1;
}

void Renderer2D::start_scene(const Matrix4f& view, const Matrix4f projection)
{
    m_data.shader.lock()->bind();

    m_data.shader.lock()->uniform_matrix4("u_projection", projection);
    m_data.shader.lock()->uniform_matrix4("u_view", view);
    m_data.shader.lock()->uniform_matrix4("u_transform", Matrix4f(1.0f));

    start_batch();
}

void Renderer2D::flush_batch()
{
    glBindVertexArray(m_data.vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_data.vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(SpriteVertex) * m_data.vertex_count, m_data.vertex_base);

    for (uint32_t i = 0; i < m_data.texture_slot_index; i++)
    {
        m_data.textures[i]->bind(i);
    }

    m_data.shader.lock()->bind();

    glDrawElements(GL_TRIANGLES, m_data.vertex_count * 6/4, GL_UNSIGNED_INT, 0);
}

void Renderer2D::render_quad(const Matrix4f& transform, const Vector3f& color)
{
    render_sprite(m_data.textures[0], transform, color, Vector2f(0, 0), Vector2f(1, 1));
}

void Renderer2D::render_sprite(const std::shared_ptr<Texture>& texture, const Matrix4f& transform, const Vector3f& color, const Vector2f& uv1, const Vector2f& uv2)
{
    float tex_index = 0.f;

    if (m_data.vertex_count >= MAX_SPRITES)
    {
        flush_batch();
        start_batch();
    }

    for (uint32_t i = 0; i < m_data.texture_slot_index; i++)
    {
        if (m_data.textures[i] == texture)
        {
            tex_index = static_cast<float>(i);
            break;
        }
    }

    if (tex_index == 0.f)
    {
        if (m_data.texture_slot_index >= 32)
        {
            flush_batch();
            start_batch();
        }

        tex_index = static_cast<float>(m_data.texture_slot_index);
        m_data.textures[m_data.texture_slot_index] = texture;
        m_data.texture_slot_index++;
    }

    const Vector2f uvs[4]
    {
        { uv1.x, uv1.y },
        { uv2.x, uv1.y },
        { uv2.x, uv2.y },
        { uv1.x, uv2.y }
    };

    m_data.vertex_ptr->pos = transform * Vector4f(0, 0, 0, 1);
    m_data.vertex_ptr->uv = uvs[0];
    m_data.vertex_ptr->color = color;
    m_data.vertex_ptr->index = tex_index;
    m_data.vertex_ptr++;

    m_data.vertex_ptr->pos = transform * Vector4f(1, 0, 0, 1);
    m_data.vertex_ptr->uv = uvs[1];
    m_data.vertex_ptr->color = color;
    m_data.vertex_ptr->index = tex_index;
    m_data.vertex_ptr++;

    m_data.vertex_ptr->pos = transform * Vector4f(1, 1, 0, 1);
    m_data.vertex_ptr->uv = uvs[2];
    m_data.vertex_ptr->color = color;
    m_data.vertex_ptr->index = tex_index;
    m_data.vertex_ptr++;

    m_data.vertex_ptr->pos = transform * Vector4f(0, 1, 0, 1);
    m_data.vertex_ptr->uv = uvs[3];
    m_data.vertex_ptr->color = color;
    m_data.vertex_ptr->index = tex_index;
    m_data.vertex_ptr++;

    m_data.vertex_count += 4;
}

void Renderer2D::render_string(const std::string& str)
{

}

void Renderer2D::finish_scene()
{
    flush_batch();
}