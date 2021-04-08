#pragma once

#include <cstdint>

class Framebuffer
{
public:
    Framebuffer(uint32_t width, uint32_t height);
    ~Framebuffer();

    void bind() const;
    void unbind() const;

    void resize(uint32_t width, uint32_t height);

    uint32_t get_color_texture() const { return m_color_id; }

private:
    uint32_t m_id, m_color_id;

    uint32_t m_width, m_height;

    void recreate();
};