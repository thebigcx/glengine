#include "engine/texture.h"
#include "engine/stb_image/stb_image.h"

#include <glad/glad.h>

Texture::Texture(const std::string& path)
    : m_path(path)
{
    stbi_set_flip_vertically_on_load(true);

    int width, height, channels;
    void* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

    glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
    glBindTextureUnit(0, m_id);

    glTextureStorage2D(m_id, 1, GL_RGBA8, width, height);
    glTextureSubImage2D(m_id, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glGenerateTextureMipmap(m_id);

    stbi_image_free(data);
    
    m_width = width;
    m_height = height;
}

Texture::Texture(uint32_t width, uint32_t height, void* data)
    : m_width(width), m_height(height)
{
    glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
    glBindTextureUnit(0, m_id);

    glTextureStorage2D(m_id, 1, GL_RGBA8, width, height);
    glTextureSubImage2D(m_id, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glGenerateTextureMipmap(m_id);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_id);
}

void Texture::bind(uint32_t slot) const
{
    glBindTextureUnit(slot, m_id);
}

void Texture::unbind(uint32_t slot) const
{
    glBindTextureUnit(slot, 0);
}