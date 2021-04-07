#pragma once

#include <string>

class Texture
{
public:
    Texture(const std::string& path);
    Texture(uint32_t width, uint32_t height);
    ~Texture();

    void bind(uint32_t slot = 0) const;
    void unbind(uint32_t slot = 0) const;

private:
    uint32_t m_id = 0;

    uint32_t m_width, m_height;
    
};