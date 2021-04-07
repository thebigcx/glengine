#include "engine/renderer_2d.h"
#include "engine/texture.h"

#include <glad/glad.h>

void Renderer2D::init()
{
    m_data.vertex_base = new SpriteVertex[MAX_SPRITES];
    m_data.vertex_current = m_data.vertex_base;

    glCreateVertexArrays(1, &m_data.vao);
    glBindVertexArray(m_data.vao);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), reinterpret_cast<void*>(offsetof(SpriteVertex, pos)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), reinterpret_cast<void*>(offsetof(SpriteVertex, uv)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), reinterpret_cast<void*>(offsetof(SpriteVertex, color)));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), reinterpret_cast<void*>(offsetof(SpriteVertex, index)));
    glEnableVertexAttribArray(3);

    glCreateBuffers(1, &m_data.vbo);
    glCreateBuffers(1, &m_data.ebo);

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

    glBindBuffer(GL_ARRAY_BUFFER, m_data.vbo);
    glBufferData(GL_ARRAY_BUFFER, MAX_VERTICES * sizeof(SpriteVertex), nullptr, GL_DYNAMIC_DRAW);
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
    m_data.vertex_current = m_data.vertex_base;
    m_data.vertex_count = 0;
    m_data.texture_slot_index = 1;
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

    glDrawElements(GL_TRIANGLES, m_data.vertex_count * 6/4, GL_UNSIGNED_INT, 0);
}

void Renderer2D::render_quad(const Vector2f& pos, const Vector2f& size, const Vector3f& color)
{

}

void Renderer2D::render_sprite(const std::shared_ptr<Texture>& texture, const Vector2f& pos, const Vector2f& size)
{
    
}

void Renderer2D::render_string(const std::string& str)
{

}