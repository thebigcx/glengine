#include "engine/sprite.h"
#include "engine/renderer_2d.h"
#include "engine/node.h"
#include "engine/texture.h"

void Sprite::on_render()
{
    if (m_texture.lock())
    {
        if (m_use_uvs)
        {
            Renderer2D::render_sprite(m_texture.lock(), m_owner->get_transform().get_translation(), 
                                      m_owner->get_transform().get_scale(), Vector3f(1, 0, 0), 
                                      m_uv1, m_uv2, m_owner->get_transform().get_rotation());
        }
        else
        {
            Vector2f uv1(0, 0);
            Vector2f uv2(m_texture.lock()->get_size().x, m_texture.lock()->get_size().y);

            Renderer2D::render_sprite(m_texture.lock(), m_owner->get_transform().get_translation(), 
                                      m_owner->get_transform().get_scale(), Vector3f(1, 0, 0), 
                                      uv1, uv2, m_owner->get_transform().get_rotation());
        }
    }
    else
    {
        Renderer2D::render_quad(m_owner->get_transform().get_translation(), m_owner->get_transform().get_scale(), 
                                m_color, m_owner->get_transform().get_rotation());
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