#pragma once

#include <string>
#include <memory>

#include "engine/maths/vector2.h"
#include "engine/maths/vector3.h"
#include "engine/maths/vector4.h"

class Texture;

class Renderer2D
{
private:
    static constexpr uint32_t MAX_SPRITES = 1000;
    static constexpr uint32_t MAX_VERTICES = MAX_SPRITES * 4;
    static constexpr uint32_t MAX_INDICES = MAX_SPRITES * 6;

    struct SpriteVertex
    {
        Vector2f pos;
        Vector2f uv;
        Vector3f color;
        float index;
    };

    static struct Renderer2DData
    {
        uint32_t vao, vbo, ebo;

        SpriteVertex* vertex_base = nullptr;
        SpriteVertex* vertex_current = nullptr;

        uint32_t vertex_count = 0;
        uint32_t texture_slot_index = 1;

        std::shared_ptr<Texture> textures[32];

    } m_data;

public:
    static void init();
    static void finalize();

    static void start_batch();
    static void flush_batch();

    static void render_quad(const Vector2f& pos, const Vector2f& size, const Vector3f& color);

    static void render_sprite(const std::shared_ptr<Texture>& texture, const Vector2f& pos, const Vector2f& size);

    static void render_string(const std::string& str);
};