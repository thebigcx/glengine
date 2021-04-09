#pragma once

#include <string>

#include "engine/maths/vector2.h"

class Texture
{
public:
    Texture(const std::string& path);
    Texture(uint32_t width, uint32_t height, void* data);
    ~Texture();

    void bind(uint32_t slot = 0) const;
    void unbind(uint32_t slot = 0) const;

    Vector2u get_size() const { return Vector2u(m_width, m_height); }

    uint32_t get_id() const { return m_id; }

    const std::string& get_path() const { return m_path; }

private:
    uint32_t m_id = 0;

    uint32_t m_width, m_height;

    std::string m_path = "";
    
};