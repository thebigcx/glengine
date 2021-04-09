#pragma once

#include <memory>

#include "engine/component.h"
#include "engine/maths/vector2.h"
#include "engine/maths/vector3.h"

class Texture;

class Sprite : public Component
{
public:
    Sprite() {}

    void on_render() override;

    const std::weak_ptr<Texture>& get_texture() const { return m_texture; }
    void set_texture(const std::weak_ptr<Texture>& texture);

    void set_uv1(const Vector2f& uv1);
    void set_uv2(const Vector2f& uv2);
    const Vector2f& get_uv1() const { return m_uv1; }
    const Vector2f& get_uv2() const { return m_uv2; }

    void set_color(const Vector3f& color);
    const Vector3f& get_color() const { return m_color; }

    bool using_uvs() const { return m_use_uvs; }
    void use_uvs(bool uvs = true) { m_use_uvs = uvs; }

private:
    Vector2f m_uv1, m_uv2;

    std::weak_ptr<Texture> m_texture;

    Vector3f m_color = { 1, 1, 1 };

    bool m_use_uvs = false;
};