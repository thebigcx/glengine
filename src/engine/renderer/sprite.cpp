#include "engine/renderer/sprite.h"
#include "engine/renderer/renderer_2d.h"
#include "engine/scene/node.h"
#include "engine/renderer/texture.h"

void Sprite::on_render()
{
    if (m_texture.lock())
    {
        if (m_use_uvs)
        {
            Vector2f uv1 = m_uv1 / Vector2f(m_texture.lock()->get_size());
            Vector2f uv2 = m_uv2 / Vector2f(m_texture.lock()->get_size());

            Renderer2D::render_sprite(m_texture.lock(), m_owner->get_transform().get_world_transform(), m_color, uv1, uv2);
        }
        else
        {
            Vector2f uv1(0, 0);
            Vector2f uv2 = m_texture.lock()->get_size();

            Renderer2D::render_sprite(m_texture.lock(), m_owner->get_transform().get_world_transform(), m_color, uv1, uv2);
        }
    }
    else
    {
        Renderer2D::render_quad(m_owner->get_transform().get_world_transform(), m_color);
    }
}

void Sprite::set_texture(const std::weak_ptr<Texture>& texture)
{
    m_texture = texture;
}

void Sprite::set_uv1(const Vector2f& uv1)
{
    m_uv1 = uv1;
}

void Sprite::set_uv2(const Vector2f& uv2)
{
    m_uv2 = uv2;
}

void Sprite::set_color(const Vector3f& color)
{
    m_color = color;
}