#include "engine/sprite.h"
#include "engine/renderer_2d.h"
#include "engine/node.h"

void Sprite::on_render()
{
    Renderer2D::render_quad(m_owner->get_transform().get_translation(), m_owner->get_transform().get_scale(), Vector3f(1, 0, 0));
}